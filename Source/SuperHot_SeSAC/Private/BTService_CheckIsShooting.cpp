// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckIsShooting.h"

#include "AIController.h"
#include "EnemyPistol.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckIsShooting::UBTService_CheckIsShooting()
{
	NodeName = TEXT("Check Is Shooting");
}

void UBTService_CheckIsShooting::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyPistol* enemy = Cast<AEnemyPistol>(OwnerComp.GetAIOwner()->GetPawn());
	if(enemy == false)
	{
		return;
	}
	
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), enemy->bIsNotShooting);
}
