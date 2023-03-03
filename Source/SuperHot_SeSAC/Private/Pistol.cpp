// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"
#include "Bullet.h"
#include "Components/SceneComponent.h"

void APistol::BeginPlay()
{
	Super::BeginPlay();
	EnemyFire();
}

void APistol::Fire()
{
	// 1초동안 시간 정상적으로 흐르도록

	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}

void APistol::EnemyFire()
{
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}



