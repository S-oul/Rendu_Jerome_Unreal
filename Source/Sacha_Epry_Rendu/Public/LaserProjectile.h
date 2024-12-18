// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LaserProjectile.generated.h"

UCLASS()
class SACHA_EPRY_RENDU_API ALaserProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALaserProjectile();

	UPROPERTY(EditAnywhere)
	float MaxTimeLife = 10.0f;
	float LifeTime = 0.0f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
