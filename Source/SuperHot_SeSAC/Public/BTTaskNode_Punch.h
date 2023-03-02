// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_Punch.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UBTTaskNode_Punch : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_Punch();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
