// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/GameModes/StarFoxGameMode.h"

#include "PlayerVessel.h"

void AStarFoxGameMode::BeginPlay()
{
	Super::BeginPlay();
	APawn* SpawnedPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
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
		FColor::Green,
		"PAWN Y"
		);
	}else return;
	
	APlayerVessel* PlayerPawn = Cast<APlayerVessel>(SpawnedPawn);
	if(PlayerPawn)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Green,
		"PLAER CAST YES"
		);
	}else return;

	USplineFollower* SplineFollower ;
	if(!PlayerPawn->SplineFollowerComponent) SplineFollower = Cast<USplineFollower>(PlayerPawn->GetComponentByClass(USplineFollower::StaticClass()));
	else SplineFollower = PlayerPawn->SplineFollowerComponent;
	
	SplineFollower->FollowSpeed = 123;
}
