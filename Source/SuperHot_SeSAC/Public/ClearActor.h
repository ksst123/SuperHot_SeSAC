// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ClearActor.generated.h"

UCLASS()
class SUPERHOT_SESAC_API AClearActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AClearActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category="Clear")
	class UStaticMeshComponent* PyramidMesh;

	UFUNCTION()
	void ToCafeMap();

	UFUNCTION()
	void GameClear();
};
