// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "EnemyBase.h"
#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SceneComponent.h"

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
	else if(weapon)
	{
		//나이아가라 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, GetActorLocation(), GetActorRotation());
		//궤적 VFX 불렛에서 분리
		trailVFX->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//총알 메쉬 제거
		bulletMeshComp->DestroyComponent();
	}
	
}

void ABullet::EnemyHitCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params;

	bool bHit = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(100.f), Params);
	if(bHit)
	{
		AEnemyBase* enemy = Cast<AEnemyBase>(HitResult.GetActor());
		if(enemy)
		{
			enemy->GetMesh()->HideBoneByName(HitResult.BoneName, EPhysBodyOp::PBO_Term);
			enemy->GetMesh()->GetPhysicsAsset();
			UE_LOG(LogTemp, Warning, TEXT("%s"), *(HitResult.BoneName.ToString()));
		}
	}
}

