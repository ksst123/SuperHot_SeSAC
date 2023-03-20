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

	AIowner = Cast<AEnemyPistol>(GetPawn());
}

void AEnemyPistolAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if(Player && AIowner)
	{
		// SetFocus(Player);
		MoveToActor(Player, 150.f);
		SetFocalPoint(Player->GetActorLocation() + FVector(10, -35.f, 0), EAIFocusPriority::Gameplay);
		// SetFocalPoint(FMath::Lerp<FVector>(owner->GetActorLocation(), Player->GetActorLocation() + FVector(10, -35.f, 0)), EAIFocusPriority::Gameplay);
	}
	
	if(Player)
	{
		// AEnemyPistol* owner = Cast<AEnemyPistol>(GetPawn());
		if(AIowner)
		{
			if(AIowner->bIsAiming == false)
			{
				AIowner->bIsAiming = true;
				// GetFocalPointOnActor(Player);
			}
		}
	}
}

FVector AEnemyPistolAIController::GetFocalPointOnActor(const AActor* Actor) const
{
	UE_LOG(LogTemp, Warning, TEXT("Focal Point"));
	return Actor != nullptr ? Actor->GetActorLocation() + FVector(0, 0, 40.f) : FAISystem::InvalidLocation;
}
