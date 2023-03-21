// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPlayer.h"

#include "Bullet.h"
#include "ClearActor.h"
#include "EnemyBase.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "FloppyDiskActor.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Pistol.h"
#include "Playpoint.h"
#include "Shotgun.h"
#include "SMG.h"
#include "SuperHotGameModeBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AHotPlayer::AHotPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 붙이기
	HotCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HotCamera"));
	HotCamera->SetupAttachment(RootComponent);
	HotCamera->bUsePawnControlRotation = true;
	// 손추가
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));
	// 손 스켈레탈메시
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.981260f,-3.500000f,4.561753f));
		LeftHandMesh->SetRelativeRotation(FRotator(-25,-180, 90));
	}
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh2 (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(TempMesh.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9f,3.5f,4.5f));
		RightHandMesh->SetRelativeRotation(FRotator(25,0,90));
	}

	HitCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Hit Collision"));
	HitCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHotPlayer::BeginPlay()
{
	Super::BeginPlay();

	gm = Cast<ASuperHotGameModeBase>(GetWorld()->GetAuthGameMode());

	RightHandMesh->OnComponentBeginOverlap.AddDynamic(this, &AHotPlayer::Punch);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.01);	
	
	// Enhanced Input 사용처리
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if(PC)
	{
		// LocalPlayer
		auto LocalPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if(subSystem)
		{
			subSystem->AddMappingContext(IMC_HotInput, 0);
			subSystem->AddMappingContext(IMC_Hand, 0);
		}
	}
	

}

// Called every frame
void AHotPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//HMD가 연결되어 있지 않다면
	if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected() == false)
	{
		//손이 카메라 방향과 일치하도록 하자
		RightHand->SetRelativeRotation(HotCamera->GetRelativeRotation());
	}
	else
	{
		PosCheck();
		// HMD, 모션 컨트롤러 속도에 따른 Time Dilation  조절
	if(!bIsFiring)
	{
		timeDilation = headVelocity/10 + RightHandVelocity/10 + LeftHandVelocity/10;
		if(timeDilation < 1.5)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), timeDilation);	
		}
		else
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);	
		}
		
	}
	
	}

	// 뭔가를 잡고 있지 않을때만
	if(!bIsGrabbedR)
	{
		// 오른손 닿은 오브젝트 머테리얼 처리
		//중심점
		//FVector Center = RightHand->GetComponentLocation();
		FVector Center = RightHandMesh->GetSocketLocation(FName("gun"));
		//충돌체크(구충돌)
		//충돌 질의 작성
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(this);
		Param.AddIgnoredComponent(RightHand);
		//충돌한 물체들 기록할 배열
		TArray<FOverlapResult> HitObjs;
		bool bHit = GetWorld()->OverlapMultiByChannel(HitObjs, Center, FQuat::Identity, ECC_GameTraceChannel18, FCollisionShape::MakeSphere(GrabRange), Param);
		
		// 충돌 안했다면
		if(!bHit)
		{
			bIsGrabbableR = false;
			
			if(GrabbedObjectR)
			{
				UE_LOG(LogTemp, Warning, TEXT("nothing grabbable"));
				GrabbedObjectR->SetMaterial(0, basicMaterial);
				GrabbedObjectR = nullptr;
				return;
			}
		}
		//가장 가까운 물체를 머테리얼을 변경하도록 하자.
		//가장 가까운 물체 인덱스
		int Closest = 0;
		
		for(int i = 0; i < HitObjs.Num(); i++)
		{
			// 물리 기능이 활성화 되어있는 녀석만 판단
			if(HitObjs[i].Component->IsSimulatingPhysics() == true)
			{
				HitObjs[i].GetComponent()->SetMaterial(0, basicMaterial);
				// 현재 손과 가장 가까운 녀석과 이번에 검출할 녀석과 더 가까운 녀석이 있다면
				//-> 필요 속성 : 현재 가장 가까운 녀석과 손과의 거리
				float distance = FVector::Dist(HitObjs[Closest].GetActor()->GetActorLocation(), Center);
				//-> 필요 속성 : 이번에 검출할 녀석과 손과의 거리
				float curDistance = FVector::Dist(HitObjs[i].GetActor()->GetActorLocation(), Center);
				// 만약 이번에가 현재꺼보다 더 가깝다면
				if(curDistance < distance)
				{
					// ->가장 가까운 녀석으로 변경하기
					Closest = i;
				}
				else
				{
					continue;
				}
				//가장 가까운 오브젝트를 GrabbedObjectR로
				GrabbedActorR = HitObjs[Closest].GetActor();
				GrabbedObjectR = HitObjs[Closest].GetComponent();
				//GrabbedObjectR의 머테리얼을 변경
				GrabbedObjectR->SetMaterial(0, grabbedMaterial);
				bIsGrabbableR = true;
			}
		}
	}
	GrabbingR();

	//클리어 됐다면
	if(gm)
	{
		if(gm->bIsCleared)
		{
			UISpawnTime += GetWorld()->DeltaTimeSeconds;
			if(!bSuperUIOn)
			{
				bSuperUIOn = true;
				//카메라 방향으로 300 거리에 SUPER UI 액터를 스폰한다.
				//GetWorld()->SpawnActor<AActor>(SuperUI, HotCamera->GetComponentLocation() + HotCamera->GetForwardVector() * 300, HotCamera->GetComponentRotation());
			}
			//1.5초 후에 HOT UI 액터를 스폰한다.
			if(UISpawnTime > 1.5 && !bHotUIOn)
			{
				bHotUIOn = true;
				//GetWorld()->SpawnActor<AActor>(HotUI, HotCamera->GetComponentLocation() + HotCamera->GetForwardVector() * 300, HotCamera->GetComponentRotation());
			}
			//3초 지나면 시간 초기화
			if(UISpawnTime > 3)
			{
				//시간 초기화
				UISpawnTime = 0;
				bSuperUIOn = false;
				bHotUIOn = false;
			}
			
			
		}
	}

		
	
}

// Called to bind functionality to input
void AHotPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if(InputSystem)
	{
		// Binding for moving
		InputSystem->BindAction(IA_HotMove, ETriggerEvent::Triggered, this, &AHotPlayer::Move);
		InputSystem->BindAction(IA_HotMove, ETriggerEvent::Completed, this, &AHotPlayer::MoveStop);
		InputSystem->BindAction(IA_HotMouse, ETriggerEvent::Triggered, this, &AHotPlayer::Turn);
		InputSystem->BindAction(IA_HotMouse, ETriggerEvent::Completed, this, &AHotPlayer::Stop);
		InputSystem->BindAction(IA_Fire, ETriggerEvent::Started, this, &AHotPlayer::Fire);
		InputSystem->BindAction(IA_GrabR, ETriggerEvent::Started, this, &AHotPlayer::TryGrabR);
		InputSystem->BindAction(IA_GrabR, ETriggerEvent::Completed, this, &AHotPlayer::DetachGrabR);
		InputSystem->BindAction(IA_GrabL, ETriggerEvent::Started, this, &AHotPlayer::TryGrabL);
		InputSystem->BindAction(IA_GrabL, ETriggerEvent::Completed, this, &AHotPlayer::DetachGrabL);
	}
}

void AHotPlayer::Move(const FInputActionValue& Values)
{
	bIsMoving = true;
	//발사 후 1초 이내에는 시간 조작 X
	//if(!bIsFiring)
	//{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	//}
	// 사용자의 입력에 따라 앞뒤좌우 이동
	FVector2D Axis = Values.Get<FVector2D>();
	if(UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected() == false)
	{
	AddMovementInput(GetActorForwardVector() , Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
	}
	else
	{
		FRotator HMDRot;
		FVector HMDDir;
		UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HMDRot, HMDDir);
		AddMovementInput(HotCamera->GetComponentRotation().Vector(), Axis.X);
		AddMovementInput(HotCamera->GetComponentRotation().Vector(), Axis.Y);
	}
	
}

void AHotPlayer::Turn(const FInputActionValue& Values)
{
	//발사 후 1초 이내에는 시간 조작 X
	//이동 중일때는 시간 조작 1로
	//if(!bIsFiring && !bIsMoving)
	//{
	//	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.1);
	//}
	FVector2d Axis = Values.Get<FVector2d>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
	
}

void AHotPlayer::Stop()
{
	if(!bIsMoving && !bIsFiring)
	{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
	}
}

void AHotPlayer::MoveStop()
{
	bIsMoving = false;
	if(!bIsFiring)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);	
	}
}

void AHotPlayer::Fire()
{
	if(bIsGrabbedR)
	{
	if(bPistolOn)
	{
		bIsFiring = true;
		// 1초동안 시간 정상적으로 흐르도록
		GetWorldTimerManager().ClearTimer(resetTimer);
		GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
		{
			bIsFiring = false;	
		}), 0.3, false);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.5);

		if(GrabbedObjectR)
		{
			if(pistol->pistolCount > 0)
			{
				//격발 사운드 스폰-----------------------------------------------------

				//머즐 플래쉬 이펙트 스폰
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, GrabbedObjectR->GetSocketLocation(TEXT("Front")), GrabbedObjectR->GetSocketRotation(TEXT("Front")));
				//총알 액터 스폰
				GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketTransform(TEXT("Front")));
				//장탄수 1 감소
				pistol->pistolCount--;
			}
			//장탄 수 없으면 격발 X
			else
			{
				//틱틱 사운드 스폰-----------------------------------------------------
				
			}
		}
	}
	else if(bSMGOn)
	{
		bIsFiring = true;	
		// 1초동안 시간 정상적으로 흐르도록
		GetWorldTimerManager().ClearTimer(resetTimer);
		GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
		{
			//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
			bIsFiring = false;		
		}), 0.3, false);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.5);
		if(GrabbedObjectR)
		{
			if(smg->smgCount > 0)
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, GrabbedObjectR->GetSocketLocation(TEXT("Front")), GrabbedObjectR->GetSocketRotation(TEXT("Front")));

				//격발 사운드 스폰-----------------------------------------------------------
				// 총알 액터 스폰
				//첫 발 발사
				GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketLocation(TEXT("Front")),GrabbedObjectR->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
				// 두 번째 발 발사
				GetWorldTimerManager().SetTimer(burstTimer1, FTimerDelegate::CreateLambda([&]()
				{
					GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketLocation(TEXT("Front")),GrabbedObjectR->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
				}), 0.1f, false);
				//세 번째 발 발사
				GetWorldTimerManager().SetTimer(burstTimer2, FTimerDelegate::CreateLambda([&]()
				{
					GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketLocation(TEXT("Front")),GrabbedObjectR->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
				}), 0.2f, false);
				smg->smgCount--;
			}
			//장탄수 0 이하
			else
			{
				//틱틱 사운드 스폰-------------------------------------------------------------
			}
		}
	
	}
	else if(bShotgunOn)
	{
		bIsFiring = true;	
		// 1초동안 시간 정상적으로 흐르도록
		GetWorldTimerManager().ClearTimer(resetTimer);
		GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
		{
			bIsFiring = false;	
		}), 0.3, false);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),0.5);
		if(GrabbedObjectR)
		{
			if(shotgun->shotgunCount > 0)
			{
				//격발 사운드 스폰 ---------------------------------------------------
				
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, GrabbedObjectR->GetSocketLocation(TEXT("Front")), GrabbedObjectR->GetSocketRotation(TEXT("Front")));
				// 총알 액터 스폰
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front1")), GrabbedObjectR->GetSocketRotation(TEXT("Front1")) + FRotator(FMath::RandRange(0,5),FMath::RandRange(0,5),0));
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front2")), GrabbedObjectR->GetSocketRotation(TEXT("Front2")) + FRotator(FMath::RandRange(0,5), 0,0));
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front3")), GrabbedObjectR->GetSocketRotation(TEXT("Front3")) + FRotator(FMath::RandRange(0,5),FMath::RandRange(0,-5),0));
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front4")), GrabbedObjectR->GetSocketRotation(TEXT("Front4")) + FRotator(0,FMath::RandRange(0,-5),0));
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front5")), GrabbedObjectR->GetSocketRotation(TEXT("Front5")) + FRotator(FMath::RandRange(0,-5),FMath::RandRange(0,-5),0));
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front6")), GrabbedObjectR->GetSocketRotation(TEXT("Front6")) + FRotator(FMath::RandRange(0,-5),FMath::RandRange(0,5),0));
				GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front7")), GrabbedObjectR->GetSocketRotation(TEXT("Front7")) + FRotator(0,FMath::RandRange(0,5),0));

				shotgun->shotgunCount--;
			}
			else
			{
				//틱틱 사운드 스폰 -------------------------------------------------
			}
			
		}
	}
	}
	
}

void AHotPlayer::TryGrabR()
{
	if(bIsGrabbableR)
	{
		if(!bIsStarted)
		{
			bIsStarted = true;
		}
		
		bIsGrabbedR = true;
		if(GrabbedObjectR)
		{
			// -> 물체 물리 기능 비활성화
			GrabbedObjectR->SetSimulatePhysics(false);
			// ->바닥 같은 오브젝트에 부딪히지 않게 처리---------------------------------------------------------------
			GrabbedObjectR->SetCollisionProfileName(FName("GrabbedWeapon"));
			// ->손에 붙혀주자
			GrabbedActorR->AttachToComponent(RightHandMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("gun"));
			// 원래 머테리얼로 돌려주기
			GrabbedObjectR->SetMaterial(0, basicMaterial);
			// 손을 안 보이게 처리
			RightHandMesh->SetVisibility(false);
		}
		
		// GrabbedObjectR이 어떤 무기 클래스냐에 따라 bool 변수 입력 변경
		pistol = Cast<APistol>(GrabbedActorR);
		smg = Cast<ASMG>(GrabbedActorR);
		shotgun = Cast<AShotgun>(GrabbedActorR);
		clearActor = Cast<AClearActor>(GrabbedActorR);
		FloppyDisk = Cast<AFloppyDiskActor>(GrabbedActorR);
		
		if(pistol)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pistol On"));
			bPistolOn = true;
		}
		else if(smg)
		{
			bSMGOn = true;
		}
		else if(shotgun)
		{
			bShotgunOn = true;
		}
		else if(clearActor)
		{
			if(bIsFirstLevel)
			{
				// 피라미드 디스트럭터블 메쉬 적용

				// 주변 구체 다시 원래크기로
				
				// n초 후에 다음 스테이지로 이동하도록
				clearActor->ToCafeMap();
				bIsFirstLevel = false;
			}
			else
			{
				clearActor->GameClear();
			}
		}
		else if(FloppyDisk)
		{
			bFloppyDisk = true;
		}
	}
	//쥘 물건이 없는 상태라면
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Punch Start"));
		bIsGrabbedR = true;
		//손이 적과 오버랩되도록
		RightHandMesh->SetCollisionProfileName(FName("Punch"));
	}
	
}


void AHotPlayer::DetachGrabR()
{
	if(bIsGrabbedR && GrabbedObjectR)
	{
			UE_LOG(LogTemp, Warning, TEXT("Right Hand Velocity : %f"), RightHandVelocity);
			GrabbedObjectR->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			GrabbedObjectR->SetSimulatePhysics(true);
			GrabbedObjectR->SetEnableGravity(true);
			//콜리전 채널 다시 바닥 등의 오브젝트와 상호작용 되도록
			RightHandMesh->SetVisibility(true);
			bPistolOn = false;
			bSMGOn = false;
			bShotgunOn = false;

			// 각속도 = DeltaTheta(특정 축 기준 변위 각도, Axis, Angle) / DeltaTime
			float Angle;
			FVector Axis;
			// 델타 로테이션으로부터 Axis와 Angle 뽑아내기
			DeltaRotationR.ToAxisAndAngle(Axis, Angle);
			float dt = GetWorld()->DeltaTimeSeconds;
			FVector AngularVelocity = (1.0f / dt) * Angle * Axis;

			//던지기 직전의 속도가 2 초과라면 던지기
			if(RightHandVelocity >2)
			{
				//콜리전 채널 Thrown으로 변경
				GrabbedObjectR->SetCollisionProfileName(FName("Thrown"));
				//던지기
				GrabbedObjectR->AddForce(ThrowDirectionR * ThrowPowerR * GrabbedObjectR->GetMass());
			}
		else
			//던지기 직전의 속도가 2 이하라면 떨구기
		{
			//콜리전 채널 Weapon으로 변경
			GrabbedObjectR->SetCollisionProfileName(FName("Weapon"));
			//아래로 떨구기
			GrabbedObjectR->AddForce(FVector(0,0,-100) * ThrowPowerR * GrabbedObjectR->GetMass());
		}
		// GrabbedObjectR 비우기, 다시 잡을 수 있게 bIsGrabbedR 뒤집기
		GrabbedObjectR->SetMaterial(0, basicMaterial);
		GrabbedObjectR = nullptr;
		bIsGrabbedR = false;
	}
		// 무기를 쥐고있던 상태가 아니라면
	else
	{
		// 주먹 충돌 해제
		UE_LOG(LogTemp, Warning, TEXT("Punch Stop"));
		bIsGrabbedR = false;
		RightHandMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}


void AHotPlayer::TryGrabL()
{
	
}


void AHotPlayer::DetachGrabL()
{
	
}

void AHotPlayer::GrabbingR()
{
	if(!bIsGrabbedR)
	{
		return;
	}
	// 던질 방향을 계속 업데이트
	ThrowDirectionR = RightHand->GetComponentLocation() - PrevPosR;
	// 회전방향 업데이트
	// 쿼터니언 공식
	// Angle1 = Q1, Angle2 = Q2
	// Angle1 + Angle 2 = Q1 * Q2
	//  -Angle1 = Q1.Inverse()
	// Angle2 - Angle1 = Q2 * Q1.Inverse()
	DeltaRotationR = RightHand->GetComponentQuat() * PrevRotR.Inverse();
	
	// 이전 위치 업데이트
	PrevPosR = RightHand->GetComponentLocation();
	// 이전 회전값 업데이트
	PrevRotR = RightHand->GetComponentQuat();
}

void AHotPlayer::GrabbingL()
{
	
}

void AHotPlayer::PosCheck()
{
	if(bIsStarted)
	{
		FXRHMDData HMDData;
		UHeadMountedDisplayFunctionLibrary::GetHMDData(GetWorld(), HMDData);
		CurHeadPos = HMDData.Position;
		HeadDirection = CurHeadPos - PrevHeadPos;
		RightHandDirection = RightHand->GetComponentLocation() - PrevPosR;
		LeftHandDirection = LeftHand->GetComponentLocation() - PrevPosL;

		//이동 속도
		headVelocity = HeadDirection.Size();
		RightHandVelocity = RightHandDirection.Size();
		LeftHandVelocity = LeftHandDirection.Size();

		//현재 위치 업데이트
		PrevHeadPos = CurHeadPos;
		PrevPosR = RightHand->GetComponentLocation();
		PrevPosL = LeftHand->GetComponentLocation();
	}
}

void AHotPlayer::Punch(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	enemy = Cast<AEnemyBase>(OtherActor);

	if(enemy)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, RightHandMesh->GetComponentLocation(), RightHandMesh->GetComponentRotation());
		enemy->Die();
	}
}