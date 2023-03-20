// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"

#include "HotPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FireDir = player->HotCamera->GetComponentLocation() - GetActorLocation();
}

void AEnemyBullet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	SetActorLocation(GetActorLocation() + FireDir* bulletSpeed * DeltaSeconds);
}
