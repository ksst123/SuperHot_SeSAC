// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"
#include "EnemyBase.h"
#include "EnemyHandFightComponent.h"
#include "HotPlayer.h"
#include "Kismet/GameplayStatics.h"

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

void UBaseEnemyAnimInstance::AnimNotify_PunchLeft()
{
	ownerEnemy->HandFightComponent->bPunching = true;
	auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(player)
	{
		player->bIsDead = true;
	}
}

void UBaseEnemyAnimInstance::AnimNotify_PunchRight()
{
	ownerEnemy->HandFightComponent->bPunching = true;
	auto player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if(player)
	{
		player->bIsDead = true;
	}
}

void UBaseEnemyAnimInstance::AnimNotify_Die()
{
	ownerEnemy->bIsDead = true;
}