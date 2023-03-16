// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyAnimInstance.h"
#include "EnemyBase.h"
#include "EnemyHandFightComponent.h"
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
	// 플레이어 구체 커진 후 첫 레벨 불러오기
	UGameplayStatics::OpenLevel(GetWorld(), FName("FenceMap"));
}

void UBaseEnemyAnimInstance::AnimNotify_PunchRight()
{
	ownerEnemy->HandFightComponent->bPunching = true;
	// 플레이어 구체 커진 후 첫 레벨 불러오기
	UGameplayStatics::OpenLevel(GetWorld(), FName("FenceMap"));
}

void UBaseEnemyAnimInstance::AnimNotify_Die()
{
	ownerEnemy->bIsDead = true;
}