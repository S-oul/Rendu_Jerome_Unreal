// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"

#include "PlayerVessel.generated.h"

UCLASS()
class SACHA_EPRY_RENDU_API APlayerVessel : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerVessel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UInputMappingContext* InputMappingContext;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetupMappingContextIntoController() const;
};
