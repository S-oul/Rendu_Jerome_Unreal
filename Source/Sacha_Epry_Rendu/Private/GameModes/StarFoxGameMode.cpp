// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/GameModes/StarFoxGameMode.h"

void AStarFoxGameMode::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Purple,
	"GameMode " 
	);
}
