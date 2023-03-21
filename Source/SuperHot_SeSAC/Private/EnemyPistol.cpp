// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPistol.h"

#include "BrainComponent.h"
#include "HotPlayer.h"
#include "Pistol.h"
#include "PistolEnemyAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyPistolAIController.h"
#include "LevelScriptActor_Cafeteria.h"
#include "Fence_LevelScriptActor.h"
#include "Components/CapsuleComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemActor.h"

AEnemyPistol::AEnemyPistol()
{

}

void AEnemyPistol::BeginPlay()
{
	Super::BeginPlay();

	bIsNotShooting = false;
	
	Pistol = GetWorld()->SpawnActor<APistol>(PistolFactory);

	if(Pistol)
	{
		Pistol->weaponMesh->SetSimulatePhysics(false);
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_rSocketPistol"));
	}

	PistolEnemyAnim = Cast<UPistolEnemyAnimInstance>(GetMesh()->GetAnimInstance());

	UE_LOG(LogTemp, Warning, TEXT("Timer Start"));
	FTimerHandle AimDelay;
	GetWorldTimerManager().SetTimer(AimDelay, ([this]()->void
	{
		bIsNotShooting = true;
		UE_LOG(LogTemp, Warning, TEXT("%s"), bIsNotShooting ? TEXT("True") : TEXT("False"));
	}
	), 0.2f, false);
	// GetWorldTimerManager().ClearTimer(AimDelay);
	UE_LOG(LogTemp, Warning, TEXT("Timer End"));
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

	bIsNotShooting = false;
	bIsAiming = false;
	PlayAnimMontage(BaseEnemyAnim->Die, 5.f, TEXT("Default"));
	BaseEnemyAnim->AnimNotify_Die();
	Pistol->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	// Pistol->weaponMesh->SetSimulatePhysics(true);
	// GetMesh()->SetSimulatePhysics(false);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// for(int i = 0; i < DestructibleMeshes.Num(); i++)
	// {
	// 	DestructibleMeshes[i]->SetSimulatePhysics(true);
	// 	// DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// }

	// GetMesh()->SetVisibility(false);
	// GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// GetCapsuleComponent()->SetSimulatePhysics(false);
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// for(int i = 0; i < DestructibleMeshes.Num(); i++)
	// {
	// 	DestructibleMeshes[i]->SetVisibility(true);
	// 	DestructibleMeshes[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// 	// enemy->GetMesh()->BreakConstraint(FVector(100.f, 100.f, 100.f), SweepResult.Location, SweepResult.BoneName);
	// 	// UE_LOG(LogTemp, Warning, TEXT("%s"), *(SweepResult.BoneName.ToString()));
	// }
	
	
	AEnemyPistolAIController* ControllerAI = Cast<AEnemyPistolAIController>(GetController());
	if(ControllerAI)
	{
		ControllerAI->BrainComponent->StopLogic(TEXT("Die"));
	}
	AFieldSystemActor* desturctionField = GetWorld()->SpawnActor<AFieldSystemActor>(DestructionField, DestructibleBody->GetComponentLocation(), DestructibleBody->GetComponentRotation());
	UnPossessed();
	
	AHotPlayer* player = Cast<AHotPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if(player)
	{
		Pistol->weaponMesh->SetSimulatePhysics(true);
		Pistol->weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		FVector PistolDir = player->GetActorLocation() - Pistol->GetActorLocation();
		Pistol->weaponMesh->AddForce(PistolDir.GetSafeNormal() * 10.f + FVector(0.f, 0.f, 150.f));
	}

	ALevelScriptActor_Cafeteria* CafeLevelBP = Cast<ALevelScriptActor_Cafeteria>(GetWorld()->GetLevelScriptActor());
	AFence_LevelScriptActor* FenceLevelBP = Cast<AFence_LevelScriptActor>(GetWorld()->GetLevelScriptActor());

	if(player->bIsFirstLevel)
	{
		if(FenceLevelBP)
		{
			FenceLevelBP->EnemyCount--;
			UE_LOG(LogTemp, Warning, TEXT("Enemy--"));
		}
	}
	else
	{
		if(CafeLevelBP)
		{
			CafeLevelBP->EnemyCount--;
			UE_LOG(LogTemp, Warning, TEXT("Cafe Enemy--"));
		}
	}
}