// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloppyDiskActor.generated.h"

UCLASS()
class SUPERHOT_SESAC_API AFloppyDiskActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloppyDiskActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* DiskMeshComponent;
};
