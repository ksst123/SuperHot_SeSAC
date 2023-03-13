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

	// 발사 처리 함수
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Fire;

	//시간 조작 변수
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bIsTimeStopped = true;
	
	UFUNCTION()
	void Stop();

	UFUNCTION()
	void MoveStop();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool bIsFiring = false;

	UPROPERTY(EditAnywhere, Category = "Input")
	bool bIsMoving = false;
	

	// 총알 발사 관련
	UFUNCTION()
	void Fire();

	UPROPERTY()
	FTimerHandle resetTimer;

	UPROPERTY()
	FTimerHandle grabResetTimer;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bPistolOn = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bSMGOn = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool bShotgunOn = false;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UNiagaraSystem* muzzleFlashVFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ABullet> bulletFactory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ABullet> bulletFactory2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ABullet> bulletFactory3;

	//  잡기용 손 스피어 생성
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* handSphereR;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* handSphereL;

	//VR 잡기 관련
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_GrabR;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_GrabL;

	void TryGrabR();
	void TryGrabL();

	void DetachGrabR();
	void DetachGrabL();
	
	UPROPERTY()
	class AWeaponBase* weapon;

	UPROPERTY(EditAnywhere, Category = "Grab")
	bool bIsGrabbableR = false;

	UPROPERTY(EditAnywhere, Category = "Grab")
	bool bIsGrabbableL = false;

	// 잡기 범위
	UPROPERTY(EditDefaultsOnly, Category = "Grab")
	float GrabRange = 20;

	// 잡고 있는지 여부
	bool bIsGrabbedR = false;

	UPROPERTY()
	class APistol* pistol;

	UPROPERTY()
	class ASMG* smg;

	FTimerHandle burstTimer1;
	FTimerHandle burstTimer2;

	UPROPERTY()
	class AShotgun* shotgun;

	UPROPERTY(EditAnywhere, Category = "Grab")
	class UMaterial* grabbedMaterial;

	UPROPERTY(EditAnywhere, Category = "Grab")
	class UMaterial* basicMaterial;

	// 오른손 닿은 물체 기억
	UPROPERTY()
	class AActor* GrabbedActorR;
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObjectR;
	// 왼손 닿은 물체 기억
	UPROPERTY()
	class UPrimitiveComponent* GrabbedObjectL;

	// 던질 방향
	FVector ThrowDirectionR;
	FVector ThrowDirectionL;
	// 던질 힘
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ThrowPowerR = 1;
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ThrowPowerL = 1;

	// 직전 위치
	FVector PrevPosR;
	FVector PrevPosL;
	// 이전 회전값
	FQuat PrevRotR;
	FQuat PrevRotL;
	// 회전 방향
	FQuat DeltaRotationR;
	FQuat DeltaRotationL;
	// 회전 빠르기
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ToquePowerR = 1;
	UPROPERTY(EditAnywhere, Category = "Grab")
	float ToquePowerL = 1;

	void GrabbingR();
	void GrabbingL();

	//머리 속도 구하기 관련
	FVector HeadDirection;
	FVector CurHeadPos;
	FVector PrevHeadPos;
	FQuat PrevHeadRot;
	FQuat DeltaRotationHead;
	
	void PosCheck();

	UPROPERTY()
	float headVelocity = 0;

	// 오른손 속도 구하기
	FVector RightHandDirection;
	float RightHandVelocity;
	FVector LeftHandDirection;
	float LeftHandVelocity;

	UPROPERTY()
	float timeDilation;
	
};
