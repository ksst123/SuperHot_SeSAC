// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Guns.generated.h"

/**
 * 
 */
UCLASS()
class SUPERHOT_SESAC_API AGuns : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UStaticMesh* bulletMesh;
};
