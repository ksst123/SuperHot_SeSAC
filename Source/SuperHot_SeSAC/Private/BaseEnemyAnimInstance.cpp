// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"

#include "EnemyBase.h"

void UBaseEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	ownerEnemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if(ownerEnemy)
	{
		FVector Velocity = ownerEnemy->GetVelocity();
		Speed = FVector::DotProduct(ownerEnemy->GetActorForwardVector(), Velocity);
	}
}
