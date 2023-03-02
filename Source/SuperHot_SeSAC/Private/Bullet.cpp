// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "WeaponBase.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bulletMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> bulletMesh (TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Weapons/bullet.bullet'"));

	if(bulletMesh.Succeeded())
	{
		bulletMeshComp->SetStaticMesh(bulletMesh.Object);
	}
	
	rotatingComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingComp"));
	rotatingComp->RotationRate = FRotator(0,0,30);
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
		OtherActor->Destroy();
		Destroy();
	}
	else if(weapon)
	{
		Destroy();
	}
	
}

