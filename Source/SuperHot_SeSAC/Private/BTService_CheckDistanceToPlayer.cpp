// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckDistanceToPlayer.h"

#include "AIController.h"
#include "EnemyBase.h"
#include "HotPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UBTService_CheckDistanceToPlayer::UBTService_CheckDistanceToPlayer()
{
	NodeName = TEXT("Check Distance To Player");
}

void UBTService_CheckDistanceToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AEnemyBase* enemy = Cast<AEnemyBase>(OwnerComp.GetAIOwner()->GetPawn());
	AHotPlayer* player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if(enemy == nullptr || player == nullptr)
	{
		return;
	}

	float dist = enemy->GetDistanceTo(player);

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GetSelectedBlackboardKey(), dist);
}
