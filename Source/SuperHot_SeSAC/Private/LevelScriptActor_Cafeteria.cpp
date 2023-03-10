// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelScriptActor_Cafeteria.h"

#include "ClearActor.h"
#include "EngineUtils.h"
#include "EnemyBase.h"
#include "HotPlayer.h"
#include "Kismet/GameplayStatics.h"

void ALevelScriptActor_Cafeteria::BeginPlay()
{
	Super::BeginPlay();

	int32 count = 0;
	for(TActorIterator<AEnemyBase> itr(GetWorld()); itr; ++itr)
	{
		Enemies.Add(*itr);
		count++;
	}

	EnemyCount = count;
	UE_LOG(LogTemp, Warning, TEXT("Enemy: %d"), EnemyCount);
}

void ALevelScriptActor_Cafeteria::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(EnemyCount == 0)
	{
		StageClear();
	}
}

void ALevelScriptActor_Cafeteria::StageClear()
{
	ClearActor = GetWorld()->SpawnActor<AClearActor>(ClearActorFactory);
	auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	ClearActor->SetActorLocation(player->GetActorLocation() + player->GetActorForwardVector() * 30.f);
}
