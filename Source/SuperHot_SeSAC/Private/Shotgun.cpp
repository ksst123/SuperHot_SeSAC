// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

#include "Bullet.h"
#include "HotPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void AShotgun::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AHotPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	Fire();
}

void AShotgun::Fire()
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
	}), 0.6, false);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),2);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), GetActorRotation());
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front1")), weaponMesh->GetSocketRotation(TEXT("Front1")) + FRotator(FMath::RandRange(0,3), FMath::RandRange(0,3), 0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front2")), weaponMesh->GetSocketRotation(TEXT("Front2")) + FRotator(FMath::RandRange(0,3), 0, 0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front3")), weaponMesh->GetSocketRotation(TEXT("Front3")) + FRotator(FMath::RandRange(0,3), FMath::RandRange(-3,0), 0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front4")), weaponMesh->GetSocketRotation(TEXT("Front4")) + FRotator(0, FMath::RandRange(-3,0), 0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front5")), weaponMesh->GetSocketRotation(TEXT("Front5")) + FRotator(FMath::RandRange(-3,0), FMath::RandRange(-3,0), 0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front6")), weaponMesh->GetSocketRotation(TEXT("Front6")) + FRotator(FMath::RandRange(-3,0), 0, 0));
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front7")), weaponMesh->GetSocketRotation(TEXT("Front7")) + FRotator(0, FMath::RandRange(0,3), 0));
	
}

void AShotgun::EnemyFire()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), weaponMesh->GetSocketRotation(TEXT("Front")));
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}