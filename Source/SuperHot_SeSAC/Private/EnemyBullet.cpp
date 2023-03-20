// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"

#include "HotPlayer.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WeaponBase.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"


void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

	bulletMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlapBegin);
	
	auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FireDir = player->HotCamera->GetComponentLocation() - GetActorLocation() + GetActorUpVector() * 30.f;
}

void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + FireDir.GetSafeNormal() * bulletSpeed * DeltaTime);
}