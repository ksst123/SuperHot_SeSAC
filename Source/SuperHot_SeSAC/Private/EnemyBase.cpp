// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "BaseEnemyAnimInstance.h"
#include "EnemyHandFightComponent.h"
#include "EnemyMoveComponent.h"
#include "Fence_LevelScriptActor.h"
#include "HotPlayer.h"
#include "LevelScriptActor_Cafeteria.h"
#include "NavigationSystemTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempSkeletal(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if(TempSkeletal.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(TempSkeletal.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
	
	MoveComponent = CreateDefaultSubobject<UEnemyMoveComponent>(TEXT("Move Component"));
	HandFightComponent = CreateDefaultSubobject<UEnemyHandFightComponent>(TEXT("Hand Fight Component"));
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	// GetMesh()->SetSimulatePhysics(true);
	// DestructibleMesh->SetWorldTransform(GetMesh()->GetComponentTransform());
	// DestructibleMesh->DestroyComponent();
	

	DestructibleBody = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destructible Body"));
	DestructibleBody->SetupAttachment(RootComponent);
	DestructibleBody->SetVisibility(false);
	DestructibleBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> TempBody(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/AI/Meshes/SM_Enemy_Body_GeometryCollection.SM_Enemy_Body_GeometryCollection'"));
	if(TempBody.Succeeded())
	{
		DestructibleBody->SetRestCollection(TempBody.Object);
		DestructibleBody->SetRelativeLocationAndRotation(FVector(0.230130, -1.931436, 27.215537), FRotator(0,-70, 10));
	}
	DestructibleMeshes.Add(DestructibleBody);
	
	DestructibleHead = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destructible Head"));
	DestructibleHead->SetupAttachment(RootComponent);
	DestructibleHead->SetVisibility(false);
	DestructibleHead->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> TempHead(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/AI/Meshes/SM_Enemy_Head_GeometryCollection.SM_Enemy_Head_GeometryCollection'"));
	if(TempHead.Succeeded())
	{
		DestructibleHead->SetRestCollection(TempHead.Object);
		DestructibleHead->SetRelativeLocationAndRotation(FVector(9.342020,2,76.939693), FRotator(0,-90,20));
	}
	DestructibleMeshes.Add(DestructibleHead);
	
	DestructibleRightArm = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destructible Right Arm"));
	DestructibleRightArm->SetupAttachment(RootComponent);
	DestructibleRightArm->SetVisibility(false);
	DestructibleRightArm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> TempRightArm(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/AI/Meshes/SM_Enemy_RightArm_GeometryCollection.SM_Enemy_RightArm_GeometryCollection'"));
	if(TempRightArm.Succeeded())
	{
		DestructibleRightArm->SetRestCollection(TempRightArm.Object);
		DestructibleRightArm->SetRelativeLocationAndRotation(FVector(-2.702382, 17.551138, 46.930447), FRotator(25.658906, -73.897886, 23.690067));
	}
	DestructibleMeshes.Add(DestructibleRightArm);
	
	DestructibleLeftArm = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destructible Left Arm"));
	DestructibleLeftArm->SetupAttachment(RootComponent);
	DestructibleLeftArm->SetVisibility(false);
	DestructibleLeftArm->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> TempLeftArm(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/AI/Meshes/SM_Enemy_LeftArm_GeometryCollection.SM_Enemy_LeftArm_GeometryCollection'"));
	if(TempLeftArm.Succeeded())
	{
		DestructibleRightArm->SetRestCollection(TempLeftArm.Object);
		DestructibleRightArm->SetRelativeLocationAndRotation(FVector(13.930650, -18.041913, 46.311575), FRotator(-25, -89.999999, 15));
	}
	DestructibleMeshes.Add(DestructibleLeftArm);
	
	DestructibleRightLeg = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destructible Right Leg"));
	DestructibleRightLeg->SetupAttachment(RootComponent);
	DestructibleRightLeg->SetVisibility(false);
	DestructibleRightLeg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> TempRightLeg(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/AI/Meshes/SM_Enemy_RightLeg_GeometryCollection.SM_Enemy_RightLeg_GeometryCollection'"));
	if(TempRightLeg.Succeeded())
	{
		DestructibleRightLeg->SetRestCollection(TempRightLeg.Object);
		DestructibleRightLeg->SetRelativeLocationAndRotation(FVector(-4.867088, 7.559282, -9.864259), FRotator(-2.497619, -40.094498, 4.332874));
	}
	DestructibleMeshes.Add(DestructibleRightLeg);
	
	DestructibleLeftLeg = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Destructible Left Leg"));
	DestructibleLeftLeg->SetupAttachment(RootComponent);
	DestructibleLeftLeg->SetVisibility(false);
	DestructibleLeftLeg->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UGeometryCollection> TempLeftLeg(TEXT("/Script/GeometryCollectionEngine.GeometryCollection'/Game/AI/Meshes/SM_Enemy_LeftLeg_GeometryCollection.SM_Enemy_LeftLeg_GeometryCollection'"));
	if(TempLeftLeg.Succeeded())
	{
		DestructibleLeftLeg->SetRestCollection(TempLeftLeg.Object);
		DestructibleLeftLeg->SetRelativeLocationAndRotation(FVector(2.317956, -11, -8.431865), FRotator(0, -90, -5));
	}
	DestructibleMeshes.Add(DestructibleLeftLeg);

	TestMeshComp = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Test Mesh Comp"));

	LeftFist = CreateDefaultSubobject<USphereComponent>(TEXT("Left Fist"));
	LeftFist->SetupAttachment(GetMesh(), TEXT("hand_lSocketFist"));
	LeftFist->SetRelativeScale3D(FVector(0.3f));
	// LeftFist->SetRelativeLocation(FVector(27.058314, 20.243555, 80.449254));
	
	RightFist = CreateDefaultSubobject<USphereComponent>(TEXT("Right Fist"));
	RightFist->SetupAttachment(GetMesh(), TEXT("hand_rSocketFist"));
	RightFist->SetRelativeScale3D(FVector(0.3f));
	// RightFist->SetRelativeLocation(FVector(-24.677729, -2.888604, 81.473991));
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	BaseEnemyAnim = Cast<UBaseEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	DestructibleBody->SetVisibility(false);
	DestructibleHead->SetVisibility(false);
	DestructibleRightArm->SetVisibility(false);
	DestructibleLeftArm->SetVisibility(false);
	DestructibleRightLeg->SetVisibility(false);
	DestructibleLeftLeg->SetVisibility(false);

	RightFist->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnBeginOverlap);
	LeftFist->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnBeginOverlap);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
}

void AEnemyBase::Die()
{
	if(bIsDead)
	{
		return;
	}
	
	// GetMesh()->SetSimulatePhysics(true);
	PlayAnimMontage(BaseEnemyAnim->Die, 5.f, TEXT("Default"));
	BaseEnemyAnim->AnimNotify_Die();
	// GetMesh()->SetSimulatePhysics(false);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetCapsuleComponent()->SetSimulatePhysics(false);
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// if(&DestructibleMeshes)
	// {
	// 	for(int i = 0; i < DestructibleMeshes.Num(); i++)
	// 	{
	// 		// 캡슐 콜리전??
	// 		// 피직스 true여야 destruction 가능
	// 		DestructibleMeshes[i]->SetSimulatePhysics(true);
	// 		// DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 	}
	// }
	// GetMesh()->SetVisibility(false);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetCapsuleComponent()->SetSimulatePhysics(false);
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// for(int i = 0; i < DestructibleMeshes.Num(); i++)
	// {
	// 	DestructibleMeshes[i]->SetVisibility(true);
	// 	DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 	// enemy->GetMesh()->BreakConstraint(FVector(100.f, 100.f, 100.f), SweepResult.Location, SweepResult.BoneName);
	// 	// UE_LOG(LogTemp, Warning, TEXT("%s"), *(SweepResult.BoneName.ToString()));
	// }
	UnPossessed();
	ALevelScriptActor_Cafeteria* CafeLevelBP = Cast<ALevelScriptActor_Cafeteria>(GetWorld()->GetLevelScriptActor());
	AFence_LevelScriptActor* FenceLevelBP = Cast<AFence_LevelScriptActor>(GetWorld()->GetLevelScriptActor());
	auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if(player->bIsFirstLevel)
	{
		if(FenceLevelBP)
		{
			FenceLevelBP->EnemyCount--;
			UE_LOG(LogTemp, Warning, TEXT("Enemy--"));
		}
	}
	else
	{
		if(CafeLevelBP)
		{
			CafeLevelBP->EnemyCount--;
		}
	}
}

void AEnemyBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHotPlayer* player = Cast<AHotPlayer>(OtherActor);
	
	if(bPunchOnce)
	{
		return;
	}
	bPunchOnce = true;

	if(player)
	{
		player->bIsDead = true;
		if(HandFightComponent->PunchDir > 50)
		{
			BaseEnemyAnim->AnimNotify_PunchLeft();
		}
		else
		{
			BaseEnemyAnim->AnimNotify_PunchRight();
		}
	}
}

void AEnemyBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

