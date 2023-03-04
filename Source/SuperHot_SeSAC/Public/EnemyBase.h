// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class SUPERHOT_SESAC_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UEnemyMoveComponent* MoveComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UEnemyHandFightComponent* HandFightComponent;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* DestructibleMesh;

	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UGeometryCollectionComponent* DestructibleLeftArm;
	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UGeometryCollectionComponent* DestructibleRightArm;
	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UGeometryCollectionComponent* DestructibleLeftLeg;
	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UGeometryCollectionComponent* DestructibleRightLeg;
	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UGeometryCollectionComponent* DestructibleBody;
	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UGeometryCollectionComponent* DestructibleHead;

	TArray<class UGeometryCollectionComponent*> DestructibleMeshes;

	UPROPERTY(VisibleAnywhere, Category = "Destructible Components")
	class UProceduralMeshComponent* TestMeshComp;
	
	class UStaticMesh* TempMesh;

	UPROPERTY()
	class UBaseEnemyAnimInstance* BaseEnemyAnim;
	
	void Die();
};
