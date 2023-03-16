// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PistolShoot.h"
#include "AIController.h"
#include "EnemyPistol.h"
#include "Pistol.h"
#include "PistolEnemyAnimInstance.h"

UBTTask_PistolShoot::UBTTask_PistolShoot()
{
	NodeName = TEXT("Shoot Pistol");
}

EBTNodeResult::Type UBTTask_PistolShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UE_LOG(LogTemp, Warning, TEXT("Shooting Node starts"));
	
	enemy = Cast<AEnemyPistol>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy)
	{
		if(enemy->bIsNotShooting)
		{
			enemy->bIsNotShooting = false;
			enemy->Pistol->EnemyFire();
		}
	}

	FTimerHandle ShootDelay;
	GetWorld()->GetTimerManager().SetTimer(ShootDelay, FTimerDelegate::CreateLambda([this]()->void
	{
		enemy->bIsNotShooting = true;
		UE_LOG(LogTemp, Warning, TEXT("Shooting true"));
	}), 1000.f, false);
	
	GetWorld()->GetTimerManager().ClearTimer(ShootDelay);

	return EBTNodeResult::Succeeded;
}
