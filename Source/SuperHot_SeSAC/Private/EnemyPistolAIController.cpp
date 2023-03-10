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

	Player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(Player)
	{
		if(BT_EnemyPistol)
		{
			RunBehaviorTree(BT_EnemyPistol);
		}
	}
}

void AEnemyPistolAIController::Tick(float DeltaSeconds)
{
	// Super::Tick(DeltaSeconds);
	
	if(Player)
	{
		SetFocus(Player);
		MoveToActor(Player, 150.f);
		SetFocalPoint(Player->GetActorLocation(), EAIFocusPriority::Default);
	}
	
	if(Player)
	{
		AEnemyPistol* owner = Cast<AEnemyPistol>(GetPawn());
		if(owner)
		{
			if(owner->bIsAiming == false)
			{
				owner->bIsAiming = true;
			}
		}
	}
}

FVector AEnemyPistolAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	return Actor != nullptr ? Actor->GetActorLocation() + FVector(0, 0, 40.f) : FAISystem::InvalidLocation;
}
