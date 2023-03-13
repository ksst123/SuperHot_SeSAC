// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "EnemyBase.h"
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
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

	//부딪힌 대상이 총알이면 서로 파괴
	if(bullet)
	{
		//나중에 산산조각 + 파티클 이펙트로 대체 필요

		//나이아가라 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, GetActorLocation(), GetActorRotation());
		//궤적 VFX 불렛에서 분리
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//총알 메쉬 제거
		bulletMeshComp->DestroyComponent();
	}
	//부딪힌 대상이 무기면 총알만 파괴
	else if(weapon)
	{
		//나이아가라 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, GetActorLocation(), GetActorRotation());
		//궤적 VFX 불렛에서 분리
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//총알 메쉬 제거
		bulletMeshComp->DestroyComponent();
	}
	//부딪힌 대상이 적이면 적을 죽이고 적 피격 나이아가라 스폰
	else if(enemy)
	{
		//나이아가라 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX2, GetActorLocation(), GetActorRotation());
		//궤적 VFX 불렛에서 분리
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//총알 메쉬 제거
		bulletMeshComp->DestroyComponent();

		enemy->Die();
		enemy->GetMesh()->SetVisibility(false);
		enemy->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		for(int i = 0; i < enemy->DestructibleMeshes.Num(); i++)
		{
			enemy->DestructibleMeshes[i]->SetVisibility(true);
			enemy->DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			enemy->GetMesh()->BreakConstraint(FVector(100.f, 100.f, 100.f), SweepResult.Location, SweepResult.BoneName);
			// UE_LOG(LogTemp, Warning, TEXT("%s"), *(SweepResult.BoneName.ToString()));
		}
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

