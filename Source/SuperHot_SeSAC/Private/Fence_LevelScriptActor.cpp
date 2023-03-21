// Fill out your copyright notice in the Description page of Project Settings.


#include "Fence_LevelScriptActor.h"
#include "SuperHotGameModeBase.h"
#include "EngineUtils.h"
#include "EnemyBase.h"


void AFence_LevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	gm = Cast<ASuperHotGameModeBase>(GetWorld()->GetAuthGameMode());

	int32 count = 0;
	for(TActorIterator<AEnemyBase> itr(GetWorld()); itr; ++itr)
	{
		Enemies.Add(*itr);
		count++;
	}

	EnemyCount = count;
	UE_LOG(LogTemp, Warning, TEXT("Enemy: %d"), EnemyCount);
}

void AFence_LevelScriptActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(EnemyCount == 0)
	{
		
		StageClear();
	}
}

void AFence_LevelScriptActor::StageClear()
{
	if(gm)
	{
			gm->bIsCleared = true;
	}
}
