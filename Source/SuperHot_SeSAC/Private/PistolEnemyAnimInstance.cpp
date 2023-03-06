// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolEnemyAnimInstance.h"
#include "EnemyPistol.h"

void UPistolEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ownerEnemyPistol = Cast<AEnemyPistol>(TryGetPawnOwner());
	if(ownerEnemyPistol)
	{
		FVector Velocity = ownerEnemyPistol->GetVelocity();
		Speed = FVector::DotProduct(ownerEnemyPistol->GetActorForwardVector(), Velocity);
		bIsAiming = ownerEnemyPistol->bIsAiming;
	}
}

void UPistolEnemyAnimInstance::AnimNotify_AimOn()
{
	ownerEnemyPistol->bIsAiming = true;
}

void UPistolEnemyAnimInstance::AnimNotify_AimOff()
{
	ownerEnemyPistol->bIsAiming = false;
}
