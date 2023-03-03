// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBaseAIController.h"
#include "HotPlayer.h"
#include "Kismet/GameplayStatics.h"

AEnemyBaseAIController::AEnemyBaseAIController()
{
	
}

void AEnemyBaseAIController::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
	{
		if(BT_EnemyBase)
		{
			RunBehaviorTree(BT_EnemyBase);
		}
	}
}

void AEnemyBaseAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// if(Player)
	// {
	// 	SetFocus(Player);
	// 	MoveToActor(Player, 150.f);
	// }
}
