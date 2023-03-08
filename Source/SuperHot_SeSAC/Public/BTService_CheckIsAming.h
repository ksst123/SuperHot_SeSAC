// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckIsAming.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UBTService_CheckIsAming : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckIsAming();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
