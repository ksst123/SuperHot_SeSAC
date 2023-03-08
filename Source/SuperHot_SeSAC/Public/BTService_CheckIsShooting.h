// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckIsShooting.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UBTService_CheckIsShooting : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckIsShooting();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
