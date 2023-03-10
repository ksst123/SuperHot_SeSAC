// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearActor.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AClearActor::AClearActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PyramidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pyramid Mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/AI/Meshes/pyramid.pyramid'"));
	if(TempMesh.Succeeded())
	{
		PyramidMesh->SetStaticMesh(TempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AClearActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AClearActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AClearActor::ToCafeMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("CafeteriaMap"));
}

void AClearActor::GameClear()
{
	
}

