// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroDesktopActor.h"

#include "FloppyDiskActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIntroDesktopActor::AIntroDesktopActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DesktopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Desktop Mesh"));
	SetRootComponent(DesktopMesh);
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/AI/Meshes/AllInOne_Computer_On.AllInOne_Computer_On'"));
	if(TempMesh.Succeeded())
	{
		DesktopMesh->SetStaticMesh(TempMesh.Object);
	}
	
	DiskEntrance = CreateDefaultSubobject<UBoxComponent>(TEXT("Disk Entrance"));
	DiskEntrance->SetupAttachment(RootComponent);
	DiskEntrance->SetBoxExtent(FVector(5.0f, 5.0f, 2.5f));


}

// Called when the game starts or when spawned
void AIntroDesktopActor::BeginPlay()
{
	Super::BeginPlay();

	DiskEntrance->OnComponentBeginOverlap.AddDynamic(this, &AIntroDesktopActor::OnBeginOverlap);
}

// Called every frame
void AIntroDesktopActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AIntroDesktopActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto FloppyDisk = Cast<AFloppyDiskActor>(OtherActor);

	if(FloppyDisk)
	{
		UGameplayStatics::OpenLevel(GetWorld(), FName("FenceMap"));
	}
}

