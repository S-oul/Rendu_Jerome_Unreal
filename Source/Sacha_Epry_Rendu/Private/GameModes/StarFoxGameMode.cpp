// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/GameModes/StarFoxGameMode.h"

#include "PlayerVessel.h"

void AStarFoxGameMode::BeginPlay()
{
	
	APlayerVessel* SpawnedPawn = GetWorld()->SpawnActor<APlayerVessel>(DefaultPawnClass, FVector::Zero(), FRotator(0, 0, 0));
	GetWorld()->GetFirstPlayerController()->Possess(SpawnedPawn);
	
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Purple,
	"GameMode " + this->GetName()
	);
	
	if(SpawnedPawn)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		"PAWN FAILED"
		);
		return;
	}
	APlayerVessel* PlayerPawn = Cast<APlayerVessel>(SpawnedPawn);
	if(PlayerPawn)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		"CAST FAILED"
		);
		return;
	}
	PlayerPawn->SplineFollowerComponent->FollowSpeed = 123;
}
