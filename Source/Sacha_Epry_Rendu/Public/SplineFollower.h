// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"
#include "SplineFollower.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SACHA_EPRY_RENDU_API USplineFollower : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USplineFollower();
	UPROPERTY(EditAnywhere)
	float FollowSpeed = 50;
	
	UPROPERTY(EditAnywhere)
	float AdvancementValue = 0;

	UPROPERTY(VisibleAnywhere)
	float AdvancementMax = 0;
	
	UPROPERTY(EditAnywhere)
	float CameraSetXDistance = 100.f;
	UPROPERTY(EditAnywhere)
	float CameraSetYDistance = 100.f;

	//Use this for the plane of the player
	FVector DirectionVector;

	FVector OldLocation;

protected:
	const FString MainSplineTag = "MainSpline";

	AActor* OwnerActor;

	UCameraComponent* MainCamera;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void AddAdvancement(float DeltaTime);

	void MoveActorToSplinePosition();
	

	void RotateActorTowardDirection();
	
	void SetCameraLocation();

	USplineComponent* Spline;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void AddPlayerInputOffset(FVector2D PlayerInputOffset);
protected:
	void InitDefaultSpline(const FString SplineTag);
};

