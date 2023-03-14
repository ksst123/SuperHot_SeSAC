// Fill out your copyright notice in the Description page of Project Settings.


#include "Playpoint.h"

#include "HotPlayer.h"
#include "Components/SphereComponent.h"

// Sets default values
APlaypoint::APlaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("sphereComp"));
}

// Called when the game starts or when spawned
void APlaypoint::BeginPlay()
{
	Super::BeginPlay();

	player = Cast<AHotPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

// Called every frame
void APlaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(player)
	{
		//플레이어가 무기를 그랩해서 시작됐다면
		if(player->bIsStarted)
		{
			//1초에 걸쳐 구체가 커지도록
			//sphereComp->SetSphereRadius()
		}
	}
}

