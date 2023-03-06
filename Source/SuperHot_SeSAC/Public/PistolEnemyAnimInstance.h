// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemyAnimInstance.h"
#include "PistolEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UPistolEnemyAnimInstance : public UBaseEnemyAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY()
	class AEnemyPistol* ownerEnemyPistol;

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PistolAimOn;
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PistolAimOff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsAiming = false;

	UFUNCTION()
	void AnimNotify_AimOn();
	UFUNCTION()
	void AnimNotify_AimOff();
};
