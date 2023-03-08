// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPistol.h"
#include "Pistol.h"
#include "PistolEnemyAnimInstance.h"

AEnemyPistol::AEnemyPistol()
{

}

void AEnemyPistol::BeginPlay()
{
	Super::BeginPlay();

	Pistol = GetWorld()->SpawnActor<APistol>(PistolFactory);

	if(Pistol)
	{
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocketPistol"));
		Pistol->SetOwner(this);
	}

	PistolEnemyAnim = Cast<UPistolEnemyAnimInstance>(GetMesh()->GetAnimInstance());
}

void AEnemyPistol::AimOn()
{
	PlayAnimMontage(PistolEnemyAnim->PistolAimOn, 1.f, TEXT("Default"));
	PistolEnemyAnim->AnimNotify_AimOn();
}

void AEnemyPistol::AimOff()
{
	PlayAnimMontage(PistolEnemyAnim->PistolAimOff, 1.f, TEXT("Default"));
	PistolEnemyAnim->AnimNotify_AimOff();
}

void AEnemyPistol::Die()
{
	Super::Die();

	bIsNotShooting = true;
	bIsAiming = false;
	PlayAnimMontage(BaseEnemyAnim->Die, 5.f, TEXT("Default"));
	BaseEnemyAnim->AnimNotify_Die();
	Pistol->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	UnPossessed();
}