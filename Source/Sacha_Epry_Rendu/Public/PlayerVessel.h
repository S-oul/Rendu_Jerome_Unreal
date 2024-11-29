// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "SplineFollower.h"
#include "GameFramework/Pawn.h"

#include "PlayerVessel.generated.h"


UCLASS()
class SACHA_EPRY_RENDU_API APlayerVessel : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerVessel();

	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region InputMappingContext 

	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveXInputAction;
	
	UPROPERTY(EditAnywhere)
	UInputAction* MoveYInputAction;

public:

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void BindInputMoveAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);
	void SetupMappingContextIntoController() const;

	void OnXMove(const FInputActionValue& InputActionValue);

	void OnYMove(const FInputActionValue& InputActionValue);

#pragma endregion

#pragma region Vessel Movement

public:
	UPROPERTY(EditAnywhere)
	float MovementSpeed;

	FVector2D PositionOffset;

	UPROPERTY(EditAnywhere)
	FVector2D MaxXYDistance;

private:
	UPROPERTY()
	USplineFollower* SplineFollowerComponent;

#pragma endregion
};
