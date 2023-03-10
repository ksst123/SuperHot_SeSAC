// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "EnemyBase.h"
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);
	
	bulletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	bulletMeshComp->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh (TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Weapons/bullet.bullet'"));

	if(bulletMesh.Succeeded())
	{
		bulletMeshComp->SetStaticMesh(bulletMesh.Object);
	}
	
	rotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComp"));
	rotatingComp->RotationRate = FRotator(0,0,30);

	trailVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BulletTrail"));
	trailVFX->SetupAttachment(bulletMeshComp);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	bulletMeshComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + GetActorForwardVector() * bulletSpeed * DeltaTime);

	EnemyHitCheck();
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bullet= Cast<ABullet>(OtherActor);
	weapon = Cast<AWeaponBase>(OtherActor);
	enemy = Cast<AEnemyBase>(OtherActor);

	//????????? ????????? ???????????? ?????? ??????
	if(bullet)
	{
		//????????? ???????????? + ????????? ???????????? ?????? ??????

		//??????????????? ??????
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, GetActorLocation(), GetActorRotation());
		//?????? VFX ???????????? ??????
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//?????? ?????? ??????
		bulletMeshComp->DestroyComponent();
	}
	//????????? ????????? ????????? ????????? ??????
	else if(weapon)
	{
		//??????????????? ??????
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, GetActorLocation(), GetActorRotation());
		//?????? VFX ???????????? ??????
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//?????? ?????? ??????
		bulletMeshComp->DestroyComponent();
	}
	//????????? ????????? ????????? ?????? ????????? ??? ?????? ??????????????? ??????
	else if(enemy)
	{
		//??????????????? ??????
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX2, GetActorLocation(), GetActorRotation());
		//?????? VFX ???????????? ??????
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//?????? ?????? ??????
		bulletMeshComp->DestroyComponent();

		enemy->GetMesh()->SetVisibility(false);
		enemy->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		enemy->GetCapsuleComponent()->SetSimulatePhysics(false);
		enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		for(int i = 0; i < enemy->DestructibleMeshes.Num(); i++)
		{
			enemy->DestructibleMeshes[i]->SetVisibility(true);
			enemy->DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			// enemy->GetMesh()->BreakConstraint(FVector(100.f, 100.f, 100.f), SweepResult.Location, SweepResult.BoneName);
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *(SweepResult.BoneName.ToString()));
		}
		enemy->Die();
	}
	else
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX2, GetActorLocation(), GetActorRotation());
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		bulletMeshComp->DestroyComponent();
	}
    
	
}

void ABullet::EnemyHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 10.f, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(10.f), Params);
	if(bHit)
	{
		enemy = Cast<AEnemyBase>(HitResult.GetActor());
		if(enemy)
		{
			// enemy->Die();
			// enemy->GetMesh()->SetVisibility(false);
			// enemy->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			// for(int i = 0; i < enemy->DestructibleMeshes.Num(); i++)
			// {
			// 	enemy->DestructibleMeshes[i]->SetVisibility(true);
			// 	enemy->DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			// }
			// enemy->GetMesh()->BreakConstraint(FVector(100.f, 100.f, 100.f), HitResult.Location, HitResult.BoneName);
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *(HitResult.BoneName.ToString()));
		}
	}
}

