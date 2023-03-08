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

	enemy = Cast<AEnemyPistol>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy)
	{
		if(enemy->bIsNotShooting)
		{
			enemy->bIsNotShooting = false;
			UE_LOG(LogTemp, Warning, TEXT("%s"), (enemy->bIsNotShooting) ? TEXT("True") : TEXT("False"));
			enemy->Pistol->EnemyFire();
		}
	}

	FTimerHandle ShootDelay;
	GetWorld()->GetTimerManager().SetTimer(ShootDelay, FTimerDelegate::CreateLambda([this]()->void
	{
		enemy->bIsNotShooting = true;
	}), 2.f, false);
	
	return EBTNodeResult::Succeeded;
}
