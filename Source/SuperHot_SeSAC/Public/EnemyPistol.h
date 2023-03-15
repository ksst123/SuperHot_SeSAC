// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "EnemyPistol.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AEnemyPistol : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyPistol();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere, Category = "Pistol")
	class APistol* Pistol;
	UPROPERTY(EditAnywhere, Category = "Pistol")
	TSubclassOf<APistol> PistolFactory;

	UPROPERTY()
	class UPistolEnemyAnimInstance* PistolEnemyAnim;

	UPROPERTY()
	bool bIsAiming = false;

	UPROPERTY()
	bool bIsNotShooting = false;

	UFUNCTION()
	void AimOn();
	UFUNCTION()
	void AimOff();

	void Die() override;
};
