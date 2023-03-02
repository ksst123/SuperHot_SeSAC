// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_IsPunching.h"
#include "EnemyBase.h"
#include "AIController.h"
#include "EnemyHandFightComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_IsPunching::UBTService_IsPunching()
{
	NodeName = TEXT("Is Punching");
}

void UBTService_IsPunching::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy == nullptr)
	{
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->HandFightComponent->bPunching);
}
