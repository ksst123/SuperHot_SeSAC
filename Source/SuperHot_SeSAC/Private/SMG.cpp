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
	}), 0.6, false);
	
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),2.0);
	
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), GetActorRotation());
	
	// 총알 액터 스폰
	//첫 발 발사
	GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front")),weaponMesh->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
	// 두 번째 발 발사
	GetWorldTimerManager().SetTimer(burstTimer1, FTimerDelegate::CreateLambda([&]()
		{
		GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front")),weaponMesh->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
		}), 0.3f, false);
	//세 번째 발 발사
	GetWorldTimerManager().SetTimer(burstTimer2, FTimerDelegate::CreateLambda([&]()
{
		GetWorld()->SpawnActor<ABullet>(bulletFactory2, weaponMesh->GetSocketLocation(TEXT("Front")),weaponMesh->GetSocketRotation(TEXT("Front")) + FRotator(FMath::RandRange(-2,2), FMath::RandRange(-2,2), 0));
	}), 0.6f, false);
}



void ASMG::EnemyFire()
{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), muzzleFlashVFX, weaponMesh->GetSocketLocation(TEXT("Front")), weaponMesh->GetSocketRotation(TEXT("Front")));
	
	// 총알 액터 스폰
	GetWorld()->SpawnActor<ABullet>(bulletFactory, weaponMesh->GetSocketTransform(TEXT("Front")));
}