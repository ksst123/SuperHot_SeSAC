// Fill out your copyright notice in the Description page of Project Settings.


#include "HotPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "SuperHotGameModeBase.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "MotionControllerComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
AHotPlayer::AHotPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 카메라 붙이기
	HotCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("HotCamera"));
	HotCamera->SetupAttachment(RootComponent);
	HotCamera->bUsePawnControlRotation = true;
	// 손추가
	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetupAttachment(RootComponent);
	LeftHand->SetTrackingMotionSource(FName("Left"));
	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetupAttachment(RootComponent);
	RightHand->SetTrackingMotionSource(FName("Right"));
	// 손 스켈레탈메시
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	if (TempMesh.Succeeded())
	{
		LeftHandMesh->SetSkeletalMesh(TempMesh.Object);
		LeftHandMesh->SetRelativeLocation(FVector(-2.981260f,-3.500000f,4.561753f));
		LeftHandMesh->SetRelativeRotation(FRotator(-25,-180, 90));
	}
	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightHand);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMesh2 (TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	if (TempMesh2.Succeeded())
	{
		RightHandMesh->SetSkeletalMesh(TempMesh.Object);
		RightHandMesh->SetRelativeLocation(FVector(-2.9f,3.5f,4.5f));
		RightHandMesh->SetRelativeRotation(FRotator(25,0,90));
	}
}

// Called when the game starts or when spawned
void AHotPlayer::BeginPlay()
{
	Super::BeginPlay();

	// Enhanced Input 사용처리
	auto PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());

	if(PC)
	{
		// LocalPlayer
		auto LocalPlayer = PC->GetLocalPlayer();
		auto subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		if(subSystem)
		{
			subSystem->AddMappingContext(IMC_HotInput, 0);
		}
	}
	
}

// Called every frame
void AHotPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AHotPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto InputSystem = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	if(InputSystem)
	{
		// Binding for moving
		InputSystem->BindAction(IA_HotMove, ETriggerEvent::Triggered, this, &AHotPlayer::Move);
		InputSystem->BindAction(IA_HotMouse, ETriggerEvent::Triggered, this, &AHotPlayer::Turn);
	}
}

void AHotPlayer::Move(const FInputActionValue& Values)
{
	// 사용자의 입력에 따라 앞뒤좌우 이동
	FVector2D Axis = Values.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Axis.X);
	AddMovementInput(GetActorRightVector(), Axis.Y);
}

void AHotPlayer::Turn(const FInputActionValue& Values)
{
	FVector2d Axis = Values.Get<FVector2d>();
	AddControllerYawInput(Axis.X);
	AddControllerPitchInput(Axis.Y);
}
