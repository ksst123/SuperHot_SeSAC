// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Punch.h"
#include "EnemyBase.h"
#include "AIController.h"
#include "EnemyHandFightComponent.h"

UBTTaskNode_Punch::UBTTaskNode_Punch()
{
	NodeName = TEXT("Punch");
}

EBTNodeResult::Type UBTTaskNode_Punch::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(!enemy)
	{
		return EBTNodeResult::Failed;
	}

	if(enemy->HandFightComponent->bPunching)
	{
		enemy->HandFightComponent->OnPunch();
	}

	enemy->HandFightComponent->bPunching = false;

	return EBTNodeResult::Succeeded;
}
