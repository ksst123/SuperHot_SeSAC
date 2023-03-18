// Fill out your copyright notice in the Description page of Project Settings.


#include "FloppyDiskActor.h"

#include "Components/BoxComponent.h"

// Sets default values
AFloppyDiskActor::AFloppyDiskActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(5.f, 5.f, 5.f));

	DiskMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Disk Mesh Component"));
	DiskMeshComponent->SetupAttachment(RootComponent);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/AI/Meshes/SM_FloppyDisk.SM_FloppyDisk'"));
	if(TempMesh.Succeeded())
	{
		DiskMeshComponent->SetStaticMesh(TempMesh.Object);
	}
}

// Called when the game starts or when spawned
void AFloppyDiskActor::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->SetSimulatePhysics(true);
}

// Called every frame
void AFloppyDiskActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

