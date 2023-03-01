// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"
#include "Bullet.h"
#include "Components/SceneComponent.h"

void APistol::Fire()
{
	GetWorld()->SpawnActor<ABullet>(bulletFactory, )
}
