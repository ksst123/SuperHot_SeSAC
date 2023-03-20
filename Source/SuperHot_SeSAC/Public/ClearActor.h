// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Clear")
	class UStaticMeshComponent* PyramidMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clear")
	class UGeometryCollectionComponent* DestructibleMesh;

	UPROPERTY()
	class ASuperHotGameModeBase* gm;
	
	UFUNCTION()
	void ToCafeMap();

	UFUNCTION()
	void GameClear();

	FTimeline timeline;
	
	UPROPERTY(EditAnywhere, Category = "Clear")
	class UCurveFloat* clearCurve;

	UFUNCTION()
	void ScaleChange(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clear")
	TSubclassOf<class AFieldSystemActor> masterField;

	UFUNCTION()
	void Grabbed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clear")
	class APlaypoint* playPoint;
	
	FTimerHandle mapTimer;
};
