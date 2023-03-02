// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHandFightComponent.h"
#include "Animation/AnimMontage.h"
#include "BaseEnemyAnimInstance.h"
#include "EnemyBase.h"

// Sets default values for this component's properties
UEnemyHandFightComponent::UEnemyHandFightComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyHandFightComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


// Called every frame
void UEnemyHandFightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyHandFightComponent::OnPunch()
{
	if(!bPunching)
	{
		return;
	}
	bPunching = false;
	
	int32 PunchDir = FMath::RandRange(1, 100);
	if(PunchDir > 50)
	{
		ownerEnemy->PlayAnimMontage(ownerEnemy->BaseEnemyAnim->PunchLeft, 2.f, TEXT("Default"));
		ownerEnemy->BaseEnemyAnim->AnimNotify_PunchLeft();
		UE_LOG(LogTemp, Warning, TEXT("Punch Left"));
	}
	else
	{
		ownerEnemy->PlayAnimMontage(ownerEnemy->BaseEnemyAnim->PunchRight, 2.f, TEXT("Default"));
		ownerEnemy->BaseEnemyAnim->AnimNotify_PunchRight();
		UE_LOG(LogTemp, Warning, TEXT("Punch Right"));
	}
}

