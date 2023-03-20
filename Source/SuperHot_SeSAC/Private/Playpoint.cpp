// Fill out your copyright notice in the Description page of Project Settings.


#include "Playpoint.h"

#include "HotPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
APlaypoint::APlaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sphereComp"));
}

// Called when the game starts or when spawned
void APlaypoint::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AHotPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());

	if(playCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &APlaypoint::ScaleChange);
		timeline.AddInterpFloat(playCurve, TimelineProgress);
	}
}

// Called every frame
void APlaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timeline.TickTimeline(DeltaTime);
	
	if(player)
	{
		//플레이어가 무기를 그랩해서 시작됐다면
		if(player->bIsStarted)
		{
			if(!bIsPlaying)
			{
				UE_LOG(LogTemp, Warning, TEXT("Play!"));
				//2초에 걸쳐 구체가 커지도록
				timeline.Play();
				bIsPlaying = true;
			}
		}
	}

	if(player->bIsDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Die"));
		GameOver();
		player->bIsDead = false;
		UGameplayStatics::OpenLevel(GetWorld(), FName("FenceMap"));
	}
}

void APlaypoint::ScaleChange(float value)
{
	sphere->SetRelativeScale3D(FVector(value));
}

void APlaypoint::GameOver()
{
	UE_LOG(LogTemp, Warning, TEXT("Game Over"));
	timeline.Reverse();
	// CurrentTime = 0;
	// GetWorldTimerManager().SetTimer(GameOverTimer, FTimerDelegate::CreateLambda([this]()->void
	// {
	// 	CurrentTime += GetWorld()->DeltaTimeSeconds;
	// 	UE_LOG(LogTemp, Warning, TEXT("CurrentTime: %f"), CurrentTime);
	// 	FVector StartPos = GetActorLocation();
	// 	FVector TargetPos = player->GetActorLocation() * player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	// 	TargetPos = FMath::Lerp<FVector>(StartPos, TargetPos, CurrentTime / GameOverTime);
	// 	SetActorLocation(StartPos);
	// 	if(CurrentTime >= GameOverTime)
	// 	{
	// 		SetActorLocation(TargetPos);
	// 		return;
	// 		
	// 	}
	// }), 0.1f, true);
	// GetWorldTimerManager().ClearTimer(GameOverTimer);
}