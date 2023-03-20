// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearActor.h"

#include "Kismet/GameplayStatics.h"
#include "SuperHotGameModeBase.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
AClearActor::AClearActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PyramidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pyramid Mesh"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/pyramid3.pyramid3'"));
	if(TempMesh.Succeeded())
	{
		PyramidMesh->SetStaticMesh(TempMesh.Object);
	}
	DestructibleMesh = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("DestructibleMesh"));
	
	

	PyramidMesh->SetSimulatePhysics(true);
	
}

// Called when the game starts or when spawned
void AClearActor::BeginPlay()
{
	Super::BeginPlay();

	gm = Cast<ASuperHotGameModeBase>(GetWorld()->GetAuthGameMode());

	DestructibleMesh->SetVisibility(false);
	DestructibleMesh->SetSimulatePhysics(false);
	DestructibleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	
	if(clearCurve)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AClearActor::ScaleChange);
		timeline.AddInterpFloat(clearCurve, TimelineProgress);
	}
}

// Called every frame
void AClearActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	timeline.TickTimeline(DeltaTime);

	if(gm->bIsCleared)
	{
		PyramidMesh->SetVisibility(true);
		timeline.Play();
		PyramidMesh->SetCollisionProfileName(FName("PhysicActor"));
	}
}

void AClearActor::ToCafeMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("CafeteriaMap"));
}

void AClearActor::GameClear()
{
	
}

void AClearActor::ScaleChange(float value)
{
	PyramidMesh->SetRelativeScale3D(FVector(value));
}

void AClearActor::Grabbed()
{
	UE_LOG(LogTemp, Warning, TEXT("Pyramid Grabbed"));
	// 피라미드 디스트럭터블 메쉬 적용
	AFieldSystemActor* field = GetWorld()->SpawnActor<class AFieldSystemActor>(masterField, DestructibleMesh->GetComponentLocation(), DestructibleMesh->GetComponentRotation());
	
	PyramidMesh->SetVisibility(false);
	PyramidMesh->SetSimulatePhysics(false);
	PyramidMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DestructibleMesh->SetVisibility(true);
	DestructibleMesh->SetSimulatePhysics(true);
	DestructibleMesh->SetCollisionProfileName(FName("Destructed"));
	DestructibleMesh->AddRadialForce(DestructibleMesh->GetComponentLocation(), 50.0f, 500.0f, RIF_Constant);
	GetWorldTimerManager().SetTimer(mapTimer, FTimerDelegate::CreateLambda([&]()
	{
		UE_LOG(LogTemp, Warning, TEXT("Next Map"));
		UGameplayStatics::OpenLevel(GetWorld(), FName("CafeteriaMap"));
	}), 1.5f, false);
}

