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
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<AEnemyBullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}