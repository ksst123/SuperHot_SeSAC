// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPistolAIController.h"

#include "EnemyPistol.h"
#include "HotPlayer.h"
#include "Kismet/GameplayStatics.h"

AEnemyPistolAIController::AEnemyPistolAIController()
{
	
}

void AEnemyPistolAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if(Player)
	{
		if(BT_EnemyPistol)
		{
			RunBehaviorTree(BT_EnemyPistol);
			UE_LOG(LogTemp, Warning, TEXT("Run BT"));
		}
	}
}

void AEnemyPistolAIController::Tick(float DeltaSeconds)
{
	// Super::Tick(DeltaSeconds);
	
	// if(Player)
	// {
	// 	SetFocus(Player);
	// 	MoveToActor(Player, 150.f);
	// }
	// if(Player)
	// {
	// 	AEnemyPistol* owner = Cast<AEnemyPistol>(GetOwner());
	// 	if(owner)
	// 	{
	// 		owner->bIsAiming = true;
	// 	}
	// }
}