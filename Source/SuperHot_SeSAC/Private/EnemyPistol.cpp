// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyPistol.h"

#include "Pistol.h"

AEnemyPistol::AEnemyPistol()
{

}

void AEnemyPistol::BeginPlay()
{
	Super::BeginPlay();

	Pistol = GetWorld()->SpawnActor<APistol>(PistolFactory);

	if(Pistol)
	{
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocketPistol"));
		Pistol->SetOwner(this);
	}
}
