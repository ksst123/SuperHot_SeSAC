// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseComponent.h"
#include "EnemyHandFightComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUPERHOT_SESAC_API UEnemyHandFightComponent : public UEnemyBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyHandFightComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	bool bPunching = true;

	UPROPERTY()
	int32 PunchDir;
	
	void OnPunch();
};
