// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_IsPunching.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UBTService_IsPunching : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_IsPunching();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
