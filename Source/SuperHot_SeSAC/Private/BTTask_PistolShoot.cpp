// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PistolShoot.h"
#include "AIController.h"
#include "EnemyPistol.h"
#include "Pistol.h"

UBTTask_PistolShoot::UBTTask_PistolShoot()
{
	NodeName = TEXT("Shoot Pistol");
}

EBTNodeResult::Type UBTTask_PistolShoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyPistol* enemy = Cast<AEnemyPistol>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy)
	{
		// enemy->Pistol->EnemyFire();
	}

	return EBTNodeResult::Succeeded;
}
