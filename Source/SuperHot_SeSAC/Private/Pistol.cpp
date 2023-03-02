// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"
#include "Bullet.h"
#include "Components/SceneComponent.h"

void APistol::BeginPlay()
{
	Super::BeginPlay();
	Fire();
}

void APistol::Fire()
{
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}

