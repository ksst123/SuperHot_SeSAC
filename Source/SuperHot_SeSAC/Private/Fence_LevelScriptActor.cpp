// Fill out your copyright notice in the Description page of Project Settings.


#include "Fence_LevelScriptActor.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyBase.h"

void AFence_LevelScriptActor::BeginPlay()
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

void AFence_LevelScriptActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(EnemyCount == 0)
	{
		// UGameplayStatics::OpenLevel(GetWorld(), FName("FenceMap"));
		UE_LOG(LogTemp, Warning, TEXT("%d: To Game Clear"), EnemyCount);
	}
}
