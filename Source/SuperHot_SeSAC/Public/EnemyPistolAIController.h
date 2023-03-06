// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseAIController.h"
#include "EnemyPistolAIController.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AEnemyPistolAIController : public AEnemyBaseAIController
{
	GENERATED_BODY()

public:
	AEnemyPistolAIController();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* BT_EnemyPistol;
};
