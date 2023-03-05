// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"
#include "Bullet.h"
#include "HotPlayer.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"

void APistol::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AHotPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	Fire();
}

void APistol::Fire()
{
	if(player)
	{
		player->bIsFiring = true;	
	}
	// 1초동안 시간 정상적으로 흐르도록
	GetWorldTimerManager().ClearTimer(resetTimer);
	GetWorldTimerManager().SetTimer(resetTimer, FTimerDelegate::CreateLambda([&]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.001);
		if(player)
		{
			player->bIsFiring = false;	
		}
		UE_LOG(LogTemp, Warning, TEXT("timerDOne"));
	}), 1, false);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1);
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}

void APistol::EnemyFire()
{
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}



