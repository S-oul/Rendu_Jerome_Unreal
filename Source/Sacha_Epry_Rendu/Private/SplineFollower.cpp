// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/SplineFollower.h"

#include "Interfaces/ITargetDevice.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USplineFollower::USplineFollower()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

	FRotator ShipRotation;

// Called when the game starts
void USplineFollower::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	
	InitSplineFollower(MainSplineTag);

	XMoveHeatValue = 0;
	YMoveHeatValue = 0;

	ShipRotation = ShipMesh->GetRelativeRotation();

}

// Called every frame
void USplineFollower::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AddAdvancement(DeltaTime);
	
	MoveActorToSplinePosition();
	RotateActorTowardDirection();

	SetCameraLocation();

	AddPlayerInputOffset();

	RotatePlayerByInputOffset(DeltaTime);
}

void USplineFollower::AddAdvancement(float DeltaTime)
{
	AdvancementValue += DeltaTime * FollowSpeed;
	AdvancementValue = FMath::Fmod(AdvancementValue,AdvancementMax);
}

void USplineFollower::MoveActorToSplinePosition()
{
	OldLocation = OwnerActor->GetActorLocation();
	
	FVector NewPosition = Spline->GetLocationAtDistanceAlongSpline(AdvancementValue,ESplineCoordinateSpace::World);

	OwnerActor->SetActorLocation(NewPosition);
}

void USplineFollower::AddPlayerInputOffset()
{
	FVector OffsetToV3 = FVector(0,PlayerInputOffset.X,PlayerInputOffset.Y);
	ShipMesh->SetRelativeLocation(OffsetToV3);
}

void USplineFollower::RotatePlayerByInputOffset(float DeltaTime)
{
	if (FMath::Abs(XMoveHeatValue) < .8f) XMoveHeatValue = 0;
	if (FMath::Abs(YMoveHeatValue) < .8f) YMoveHeatValue = 0;
	 
	if (XMoveHeatValue != 0) XMoveHeatValue += FMath::Sign(XMoveHeatValue) * -.6f;
	if (YMoveHeatValue != 0) YMoveHeatValue += FMath::Sign(YMoveHeatValue) * -.6f;

	YMoveHeatValue = FMath::Clamp(YMoveHeatValue, -2, 2);
	XMoveHeatValue = FMath::Clamp(XMoveHeatValue, -2, 2);
	
	ShipMesh->SetRelativeRotation(FRotator(ShipRotation.Pitch, ShipRotation.Yaw + XMoveHeatValue * 10, ShipRotation.Roll + YMoveHeatValue * 10));
}

void USplineFollower::RotateActorTowardDirection()
{
	FVector ForwardDir =  OwnerActor->GetActorLocation() - OldLocation ;
	ForwardDir.Normalize();
	DirectionVector = ForwardDir;
	
	FRotator NewRotation = FRotationMatrix::MakeFromX(ForwardDir).Rotator();
	
	OwnerActor->SetActorRotation(NewRotation);
}

void USplineFollower::SetCameraLocation()
{
	if(MainCamera == nullptr) return;
	
	MainCamera->SetWorldLocation(OwnerActor->GetActorLocation() - DirectionVector * CameraSetXDistance + FVector::UpVector * CameraSetYDistance);
	MainCamera->SetWorldRotation(OwnerActor->GetActorRotation());
} 

void USplineFollower::SetPlayerInputOffset(const FVector2D InputOffset, float HeatXMove, float HeatYMove)
{
	PlayerInputOffset = InputOffset;
	XMoveHeatValue += HeatXMove; 
	YMoveHeatValue += HeatYMove; 
}

void USplineFollower::InitSplineFollower(const FString SplineTag)
{
	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), *SplineTag, AllActor);

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

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "MainCamera", AllActor);
	MainCamera = AllActor.Num() > 0 ? AllActor[0]->GetComponentByClass<UCameraComponent>() : nullptr;
	if(MainCamera == nullptr)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		"No Camera Found !"
		);
		return;
	}

	OwnerActor->GetComponents<UArrowComponent>(Arrows);
	if(Arrows.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		"No Arrows Found !"
		);
		return;
	}

	TArray<UStaticMeshComponent*> Allmesh;
	OwnerActor->GetComponents<UStaticMeshComponent>(Allmesh);
	if(Allmesh.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		"No Mesh Found !"
		);
		return;
	}
	
	ShipMesh = Allmesh[0];

	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Green,
	Spline->GetName() + " " + FString::FromInt(AdvancementMax) + " \n" + MainCamera->GetName() 
	 );
}

//EDITOR


void USplineFollower::PrintLengthOfSpline()
{
	GEngine->AddOnScreenDebugMessage
		(
		-1,
		10.f,
		FColor::Red,
		FString::SanitizeFloat(Spline->GetSplineLength())
		);
}

