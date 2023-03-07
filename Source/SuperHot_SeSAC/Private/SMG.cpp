// Fill out your copyright notice in the Description page of Project Settings.


#include "SMG.h"
#include "HotPlayer.h"
#include "Bullet.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

void ASMG::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<AHotPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	Fire();
}

void ASMG::Fire()
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
		UE_LOG(LogTemp, Warning, TEXT("timerDone"));
	}), 0.5, false);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),1.5);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), GetActorRotation());
	
	// 총알 액터 스폰
	float randPitch = FMath::RandRange(-3,3);
	float randYaw = FMath::RandRange(-3, 3);
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front")),weaponMesh->GetSocketRotation(TEXT("Front")) + FRotator(randPitch, randYaw, 0));
}

void ASMG::EnemyFire()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), weaponMesh->GetSocketRotation(TEXT("Front")));
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}