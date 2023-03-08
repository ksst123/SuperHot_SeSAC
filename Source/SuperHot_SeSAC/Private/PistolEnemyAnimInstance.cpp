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

		HorizontalAngle = ownerEnemyPistol->GetBaseAimRotation().Pitch - 180;
		VerticalAngle = ownerEnemyPistol->GetBaseAimRotation().Yaw - 180;
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

// void UPistolEnemyAnimInstance::AnimNotify_ShootEnd()
// {
// 	ownerEnemyPistol->bIsNotShooting = true;
// }
