// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "BaseEnemyAnimInstance.h"
#include "EnemyHandFightComponent.h"
#include "EnemyMoveComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "ProceduralMeshComponent.h"

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
		DestructibleLeftArm->SetRestCollection(TempLeftArm.Object);
		DestructibleLeftArm->SetRelativeLocationAndRotation(FVector(13.930650, -18.041913, 46.311575), FRotator(-25, -89.999999, 15));
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
	GetMesh()->SetSimulatePhysics(true);
}

