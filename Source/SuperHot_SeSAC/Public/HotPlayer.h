// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "HotPlayer.generated.h"

UCLASS()
class SUPERHOT_SESAC_API AHotPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHotPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 카메라
	UPROPERTY(VisibleAnywhere, Category="VRCamera")
	class UCameraComponent* HotCamera;

	// 컨트롤러
	UPROPERTY(VisibleAnywhere, Category="MotionController")
	class UMotionControllerComponent* LeftHand;
	UPROPERTY(VisibleAnywhere, Category="MotionController")
	class UMotionControllerComponent* RightHand;
	// 사용할 손 모델
	UPROPERTY(VisibleAnywhere, Category="MotionController")
	class USkeletalMeshComponent* LeftHandMesh;	
	UPROPERTY(VisibleAnywhere, Category="MotionController")
	class USkeletalMeshComponent* RightHandMesh;
	
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 이동속도
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float MoveSpeed = 500;
	// Input Mapping Context
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_HotInput;
	// Input Action for Move
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_HotMove;
	
	// 움직임처리 함수
	void Move(const FInputActionValue& Values);

	// 마우스 입력처리
	UPROPERTY(EditDefaultsOnly, Category="Input")
	class UInputAction* IA_HotMouse;
	void Turn(const FInputActionValue& Values);

	//시간 조작 변수
	UPROPERTY(EditAnywhere, Category = "Input")
	float moveDilationX;

	UPROPERTY(EditAnywhere, Category = "Input")
	float moveDilationY;


	UPROPERTY(EditAnywhere, Category = "Input")
	float turnDilation;

	UPROPERTY(EditAnywhere, Category = "Input")
	float timeDilation;

	UPROPERTY(EditAnywhere, Category = "Input")
	bool bIsTimeStopped = true;

	UFUNCTION()
	void Stop();
};
