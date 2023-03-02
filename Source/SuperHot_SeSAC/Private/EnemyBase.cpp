// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "BaseEnemyAnimInstance.h"
#include "EnemyHandFightComponent.h"
#include "EnemyMoveComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	DestructibleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Destructible Mesh"));
	DestructibleMesh->SetupAttachment(RootComponent);
	DestructibleMesh->SetVisibility(false);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	TempMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/AI/SM_Enemy.SM_Enemy'"));
	DestructibleMesh->SetStaticMesh(TempMesh);

	// GetMesh()->SetSimulatePhysics(true);
	// DestructibleMesh->SetWorldTransform(GetMesh()->GetComponentTransform());
	// DestructibleMesh->DestroyComponent();
	

	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	BaseEnemyAnim = Cast<UBaseEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	FTimerHandle TimerHandle;
	float DelaySeconds = 1.0f;
	
	GetMesh()->SetSimulatePhysics(true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]
	{
		DestructibleMesh->SetWorldTransform(GetMesh()->GetComponentTransform());
		DestructibleMesh->SetVisibility(true);
		DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GetMesh()->DestroyComponent();
	}
		, DelaySeconds, false);
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

