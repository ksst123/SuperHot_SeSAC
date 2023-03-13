// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Fence_LevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AFence_LevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyCount = -1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AEnemyBase*> Enemies;
};
