// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AShotgun : public AWeaponBase
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

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UNiagaraSystem* muzzleFlashVFX;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	int shotgunCount = 2;
};
