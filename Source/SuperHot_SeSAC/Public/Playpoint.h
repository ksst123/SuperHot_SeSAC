// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Playpoint.generated.h"

UCLASS()
class SUPERHOT_SESAC_API APlaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaypoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* sphere;
	
	UPROPERTY(EditAnywhere, Category = "Play")
	class AHotPlayer* player;

	UPROPERTY(EditAnywhere, Category = "Play")
	bool bIsPlaying = false;

	FTimeline timeline;
	
	UPROPERTY(EditAnywhere, Category = "Play")
	class UCurveFloat* playCurve;

	UFUNCTION()
	void ScaleChange(float value);
};
