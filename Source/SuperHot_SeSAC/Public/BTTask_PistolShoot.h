// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PistolShoot.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UBTTask_PistolShoot : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PistolShoot();

	UPROPERTY()
	class AEnemyPistol* enemy;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
