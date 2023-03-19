// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "EnemyBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	weaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("weaponMesh"));

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	weaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::Crash);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Crash(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OVerlapped Something"));
	enemy = Cast<AEnemyBase>(OtherActor);
	//오버랩된 액터가 에너미라면
	// 적을 Die 시키고
	// 총을 없앤다.
	if(enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy Crashed"));
		enemy->Die();
		//에너미 피격 나이아가라 스폰
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), bulletVFX, GetActorLocation(), GetActorRotation());
	}
	else
	//오버랩된 액터가 오브젝트라면
	//총만 없앤다.
	{
		UE_LOG(LogTemp, Warning, TEXT("Object Crashed"));
		//디스트럭터블 메쉬 추가 필요----------------------------------------------------------------------
		Destroy();
	}
	
	
}


