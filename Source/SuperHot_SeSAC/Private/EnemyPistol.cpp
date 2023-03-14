// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPistol.h"

#include "BrainComponent.h"
#include "HotPlayer.h"
#include "Pistol.h"
#include "PistolEnemyAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPistolAIController.h"
#include "LevelScriptActor_Cafeteria.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AEnemyPistol::AEnemyPistol()
{

}

void AEnemyPistol::BeginPlay()
{
	Super::BeginPlay();

	Pistol = GetWorld()->SpawnActor<APistol>(PistolFactory);

	if(Pistol)
	{
		Pistol->weaponMesh->SetSimulatePhysics(false);
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocketPistol"));
	}

	PistolEnemyAnim = Cast<UPistolEnemyAnimInstance>(GetMesh()->GetAnimInstance());
}

void AEnemyPistol::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
	// Super::Die();
	if(bIsDead)
	{
		return;
	}

	bIsNotShooting = true;
	bIsAiming = false;
	PlayAnimMontage(BaseEnemyAnim->Die, 5.f, TEXT("Default"));
	BaseEnemyAnim->AnimNotify_Die();
	Pistol->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// GetMesh()->SetSimulatePhysics(false);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	for(int i = 0; i < DestructibleMeshes.Num(); i++)
	{
		DestructibleMeshes[i]->SetSimulatePhysics(true);
		DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	UnPossessed();
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

	ALevelScriptActor_Cafeteria* LevelBP = Cast<ALevelScriptActor_Cafeteria>(GetWorld()->GetLevelScriptActor());
	if(LevelBP)
	{
		LevelBP->EnemyCount--;
		UE_LOG(LogTemp, Warning, TEXT("%d enemy"), LevelBP->EnemyCount);
	}

	FTimerHandle DestroyDelay;
	GetWorld()->GetTimerManager().SetTimer(DestroyDelay, FTimerDelegate::CreateLambda([this]()->void
	{
		for(int i = 0; i < DestructibleMeshes.Num(); i++)
		{
			DestructibleMeshes[i]->DestroyComponent();
		}
	}), 2.f, false);
	GetWorld()->GetTimerManager().ClearTimer(DestroyDelay);
}