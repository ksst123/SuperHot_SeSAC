// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY()
	class AEnemyBase* ownerEnemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float Speed;

	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PunchLeft;
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* PunchRight;
	UPROPERTY(EditAnywhere, Category = "Montages")
	class UAnimMontage* Die;

	UFUNCTION()
	void AnimNotify_PunchLeft();
	UFUNCTION()
	void AnimNotify_PunchRight();
	UFUNCTION()
	void AnimNotify_Die();
};
