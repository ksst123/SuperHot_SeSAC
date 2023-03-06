// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()

public:
	AEnemyBaseAIController();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditDefaultsOnly)
	class UBehaviorTree* BT_EnemyBase;

	UPROPERTY()
	class AHotPlayer* Player;
};
