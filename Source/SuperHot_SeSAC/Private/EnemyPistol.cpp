// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPistol.h"

#include "BrainComponent.h"
#include "HotPlayer.h"
#include "Pistol.h"
#include "PistolEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPistolAIController.h"

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
	}

	PistolEnemyAnim = Cast<UPistolEnemyAnimInstance>(GetMesh()->GetAnimInstance());
}

void AEnemyPistol::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(Pistol)
	{
		// Pistol->
	}
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
	Pistol->weaponMesh->SetSimulatePhysics(true);
	AEnemyPistolAIController* ControllerAI = Cast<AEnemyPistolAIController>(GetController());
	if(ControllerAI)
	{
		ControllerAI->BrainComponent->StopLogic(TEXT("Die"));
	}
	
	
	AHotPlayer* player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(player)
	{
		Pistol->weaponMesh->SetSimulatePhysics(true);
		Pistol->weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Pistol->weaponMesh->AddForce((player->GetActorLocation() - Pistol->GetActorLocation() + FVector(0.f, 0.f, 150.f))  * 100.f);
	}
}