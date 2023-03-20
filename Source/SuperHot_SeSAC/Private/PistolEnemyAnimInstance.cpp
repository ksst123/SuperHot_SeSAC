// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolEnemyAnimInstance.h"
#include "EnemyPistol.h"
#include "HotPlayer.h"
#include "Kismet/GameplayStatics.h"

void UPistolEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	ownerEnemyPistol = Cast<AEnemyPistol>(TryGetPawnOwner());
	if(ownerEnemyPistol)
	{
		FVector Velocity = ownerEnemyPistol->GetVelocity();
		Speed = FVector::DotProduct(ownerEnemyPistol->GetActorForwardVector(), Velocity);
		bIsAiming = ownerEnemyPistol->bIsAiming;
		
		VerticalAngle = ownerEnemyPistol->GetBaseAimRotation().Pitch;
		HorizontalAngle = ownerEnemyPistol->GetBaseAimRotation().Yaw;
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
