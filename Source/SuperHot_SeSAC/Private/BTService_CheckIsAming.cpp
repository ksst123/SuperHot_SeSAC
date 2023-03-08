// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckIsAming.h"

#include "AIController.h"
#include "EnemyPistol.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckIsAming::UBTService_CheckIsAming()
{
	NodeName = TEXT("Check Is Aiming");
}

void UBTService_CheckIsAming::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyPistol* enemy = Cast<AEnemyPistol>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy == false)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->bIsAiming);
}
