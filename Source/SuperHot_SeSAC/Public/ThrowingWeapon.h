// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ThrowingWeapon.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AThrowingWeapon : public AWeaponBase
{
	GENERATED_BODY()

public:
	void Crash(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) __override;

};
