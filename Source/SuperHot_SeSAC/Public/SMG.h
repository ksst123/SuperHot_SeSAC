// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "SMG.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API ASMG : public AWeaponBase
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void EnemyFire();

	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AHotPlayer* player;

	UPROPERTY()
	FTimerHandle resetTimer;

	UPROPERTY()
	FTimerHandle burstTimer;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UNiagaraSystem* muzzleFlashVFX;
	
};
