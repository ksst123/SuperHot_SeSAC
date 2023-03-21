// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraSystem.h"
#include "Bullet.generated.h"

UCLASS()
class SUPERHOT_SESAC_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UStaticMeshComponent* bulletMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float bulletSpeed = 1200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class URotatingMovementComponent* rotatingComp;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class ABullet* bullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AWeaponBase* weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class AEnemyBase* enemy;

	void EnemyHitCheck();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapong")
	class UNiagaraComponent* trailVFX;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* bulletVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	class UNiagaraSystem* bulletVFX2;

	UPROPERTY(EditAnywhere, Category="Weapon")
	class UBoxComponent* BoxComponent;

	UPROPERTY()
	FVector FireDir;

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* BulletSound;
};
