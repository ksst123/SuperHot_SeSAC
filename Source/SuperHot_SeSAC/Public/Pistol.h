// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API APistol : public AWeaponBase
{
	GENERATED_BODY()

public:
	void Fire();
	
	void EnemyFire();

	virtual void BeginPlay() override;
};