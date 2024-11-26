// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/SplineFollower.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USplineFollower::USplineFollower()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USplineFollower::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	
	InitDefaultSpline(MainSplineTag);
}

void USplineFollower::AddAdvancement(float DeltaTime)
{
	AdvancementValue += DeltaTime * FollowSpeed;
	AdvancementValue = FMath::Fmod(AdvancementValue,AdvancementMax);
}

void USplineFollower::MoveActorToSplinePosition()
{
	FVector NewPosition = Spline->GetLocationAtDistanceAlongSpline(AdvancementValue,ESplineCoordinateSpace::World);

	OwnerActor->SetActorLocation(NewPosition);
}

void USplineFollower::RotateActorTowardDirection()
{
	FRotator CurrentRotation = OwnerActor->GetActorRotation();

	FVector ForwardDir = Spline->GetLocationAtDistanceAlongSpline(AdvancementValue,ESplineCoordinateSpace::World) - Spline->GetLocationAtDistanceAlongSpline(AdvancementValue-1,ESplineCoordinateSpace::World);
	FRotator NewRotation = FRotationMatrix::MakeFromX(ForwardDir).Rotator();
	
	OwnerActor->SetActorRotation(NewRotation);
}


// Called every frame
void USplineFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddAdvancement(DeltaTime);
	
	MoveActorToSplinePosition();
	RotateActorTowardDirection();
}

void USplineFollower::InitDefaultSpline(const FString SplineTag)
{
	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), *SplineTag, AllActor);
	GEngine->AddOnScreenDebugMessage
			(
			-1,
			10.f,
			FColor::Purple,
			FString::FromInt(AllActor.Num())
			);

	Spline = AllActor.Num() > 0 ? AllActor[0]->GetComponentByClass<USplineComponent>() : nullptr;
	if(Spline == nullptr)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		"No Spline Found !"
		);
		return;
	}

	AdvancementMax = Spline->GetSplineLength();
	
	
	GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Green,
		Spline->GetName() + " " + FString::FromInt(AdvancementMax)
		);
}

