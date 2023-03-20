// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "HotPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

void APistol::BeginPlay()
{
	Super::BeginPlay();
}

void APistol::EnemyFire()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), weaponMesh->GetSocketRotation(TEXT("Front")));

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if(pistolCount > 0)
	{
		// 총알 액터 스폰
		GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")), SpawnParams);
		UE_LOG(LogTemp, Warning, TEXT("Enemy Bullet Spawned"));
		pistolCount--;
	}
	else
	{
		
	}
}