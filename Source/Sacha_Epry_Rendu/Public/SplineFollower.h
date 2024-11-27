// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

	FVector DirectionVector;

	FVector OldLocation;

protected:
	const FString MainSplineTag = "MainSpline";

	AActor* OwnerActor;
	
	// Called when the game starts
	virtual void BeginPlay() override;

	void AddAdvancement(float DeltaTime);

	void MoveActorToSplinePosition();

	void RotateActorTowardDirection();
	
	
	USplineComponent* Spline;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitDefaultSpline(const FString SplineTag);

};

