// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPlayer.h"

#include "Bullet.h"
#include "EnhancedInputSubsystems.h"
#include "SuperHotGameModeBase.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Pistol.h"
#include "Shotgun.h"
#include "SMG.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
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

}

// Called when the game starts or when spawned
void AHotPlayer::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.05);

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
		// HMD, 모션 컨트롤러 속도에 따른 Time Dilation  조절
		FXRHMDData HMDData;
		UHeadMountedDisplayFunctionLibrary::GetHMDData(GetWorld(), HMDData);
		FVector currentHMDPos = HMDData.Position;
		
		
		//UGameplayStatics::SetGlobalTimeDilation(GetWorld(), );
	
	}

	// 뭔가를 잡고 있지 않을때만
	if(!bIsGrabbedR)
	{
		// 오른손 닿은 오브젝트 머테리얼 처리
		//중심점
		FVector Center = RightHand->GetComponentLocation();
		//충돌체크(구충돌)
		//충돌 질의 작성
		FCollisionQueryParams Param;
		Param.AddIgnoredActor(this);
		Param.AddIgnoredComponent(RightHand);
		//충돌한 물체들 기록할 배열
		TArray<FOverlapResult> HitObjs;
		bool bHit = GetWorld()->OverlapMultiByChannel(HitObjs, Center, FQuat::Identity, ECC_GameTraceChannel15, FCollisionShape::MakeSphere(GrabRange), Param);
		
		// 충돌했다면
		if(!bHit)
		{
			bIsGrabbableR = false;
			if(GrabbedObjectR)
			{
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
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
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
	if(bPistolOn)
	{
		bIsFiring = true;
		// 1초동안 시간 정상적으로 흐르도록
		GetWorldTimerManager().ClearTimer(resetTimer);
		GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
			bIsFiring = false;	
		}), 0.6, false);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),2);
	
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, GrabbedObjectR->GetSocketLocation(TEXT("Front")), GrabbedObjectR->GetSocketRotation(TEXT("Front")));
	
		// 총알 액터 스폰
		GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketTransform(TEXT("Front")));
	}
	else if(bSMGOn)
	{
		bIsFiring = true;	
		// 1초동안 시간 정상적으로 흐르도록
		GetWorldTimerManager().ClearTimer(resetTimer);
		GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
			bIsFiring = false;		
		}), 0.6, false);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),2.0);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, GrabbedObjectR->GetSocketLocation(TEXT("Front")), GrabbedObjectR->GetSocketRotation(TEXT("Front")));
	
	// 총알 액터 스폰
	//첫 발 발사
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketLocation(TEXT("Front")),GrabbedObjectR->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
	// 두 번째 발 발사
	GetWorldTimerManager().SetTimer(burstTimer1, FTimerDelegate::CreateLambda([&]()
		{
		GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketLocation(TEXT("Front")),GrabbedObjectR->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
		}), 0.15f, false);
	//세 번째 발 발사
	GetWorldTimerManager().SetTimer(burstTimer2, FTimerDelegate::CreateLambda([&]()
{
		GetWorld()->SpawnActor<ABullet>(bulletFactory2, GrabbedObjectR->GetSocketLocation(TEXT("Front")),GrabbedObjectR->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
	}), 0.3f, false);
	}
	else if(bShotgunOn)
	{
		bIsFiring = true;	
		// 1초동안 시간 정상적으로 흐르도록
		GetWorldTimerManager().ClearTimer(resetTimer);
		GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
			bIsFiring = false;	
		}), 0.6, false);
	
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(),2);

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, GrabbedObjectR->GetSocketLocation(TEXT("Front")), GrabbedObjectR->GetSocketRotation(TEXT("Front")));
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front1")), GrabbedObjectR->GetSocketRotation(TEXT("Front1")) + FRotator(FMath::RandRange(0,5),FMath::RandRange(0,5),0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front2")), GrabbedObjectR->GetSocketRotation(TEXT("Front2")) + FRotator(FMath::RandRange(0,5), 0,0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front3")), GrabbedObjectR->GetSocketRotation(TEXT("Front3")) + FRotator(FMath::RandRange(0,5),FMath::RandRange(0,-5),0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front4")), GrabbedObjectR->GetSocketRotation(TEXT("Front4")) + FRotator(0,FMath::RandRange(0,-5),0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front5")), GrabbedObjectR->GetSocketRotation(TEXT("Front5")) + FRotator(FMath::RandRange(0,-5),FMath::RandRange(0,-5),0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front6")), GrabbedObjectR->GetSocketRotation(TEXT("Front6")) + FRotator(FMath::RandRange(0,-5),FMath::RandRange(0,5),0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory3, GrabbedObjectR->GetSocketLocation(TEXT("Front7")), GrabbedObjectR->GetSocketRotation(TEXT("Front7")) + FRotator(0,FMath::RandRange(0,5),0));
	}
	
}

void AHotPlayer::TryGrabR()
{
	if(bIsGrabbableR)
	{
		bIsGrabbedR = true;
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

		// GrabbedObjectR이 어떤 무기 클래스냐에 따라 bool 변수 입력 변경
		pistol = Cast<APistol>(GrabbedActorR);
		smg = Cast<ASMG>(GrabbedActorR);
		shotgun = Cast<AShotgun>(GrabbedActorR);
		
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
		
	}
	else
	{
		//쥘수 없는 상태라면
		//주먹 쥐는 애니메이션 추가
		
	}
	
}


void AHotPlayer::DetachGrabR()
{
	if(bIsGrabbedR)
	{
		bIsGrabbedR = false;
		if(GrabbedObjectR)
		{
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
			
			GetWorldTimerManager().ClearTimer(resetTimer);
			GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
			}), 0.3f, false);
			
			GrabbedObjectR->SetSimulatePhysics(true);
			//콜리전 채널 다시 바닥 등의 오브젝트와 상호작용 되도록
			GrabbedObjectR->SetCollisionProfileName(FName("PhysicsActor"));
			GrabbedObjectR->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
			RightHandMesh->SetVisibility(true);
			bPistolOn = false;
			bSMGOn = false;
			bShotgunOn = false;
		}
	}
	else
	{
		// 무기를 쥐고있던 상태가 아니라면
		
		// 주먹 해제
	}
	
}


void AHotPlayer::TryGrabL()
{
	
}


void AHotPlayer::DetachGrabL()
{
	
}

