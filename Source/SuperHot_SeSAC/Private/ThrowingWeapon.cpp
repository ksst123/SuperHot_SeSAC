// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowingWeapon.h"

#include "EnemyBase.h"

// void AThrowingWeapon::Crash(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
//                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("OVerlapped Something"));
// 	enemy = Cast<AEnemyBase>(OtherActor);
// 	//오버랩된 액터가 에너미라면
// 	// 적을 Die 시키고
// 	// 총을 없앤다.
// 	if(enemy)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Enemy Crashed"));
// 		enemy->Die();
// 		//디스럭터블 메쉬 추가 필요 ------------------------------------------------------------------------   
// 		Destroy();
// 	}
// 	else
// 		//오버랩된 액터가 오브젝트라면
// 		//총만 없앤다.
// 		{
// 		UE_LOG(LogTemp, Warning, TEXT("Object Crashed"));
// 		//디스트럭터블 메쉬 추가 필요----------------------------------------------------------------------
// 		Destroy();
// 		}
// }
