// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/PlayerVessel.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Chaos/ChaosPerfTest.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerVessel::APlayerVessel()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerVessel::BeginPlay()
{
	Super::BeginPlay();
	SplineFollower = Cast<USplineFollower>(GetComponentByClass(USplineFollower::StaticClass()));

	if(SplineFollower == nullptr)
	{
		GEngine->AddOnScreenDebugMessage
	(
	-1,
	2,
	FColor::Red,
	"NOSPLINEFOLLO"
	);
		return;
	}
	
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	2,
	FColor::Purple,
	SplineFollower->GetName()
	);
	
}


// Called every frame
void APlayerVessel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	DeltaTime,
	FColor::Yellow,
	FString::Printf(TEXT("X: %f Y: %f"), PositionOffset.X,PositionOffset.Y)
	);

	SplineFollower->AddPlayerInputOffset(PositionOffset);

	
}
#pragma region InputMappingContext

void APlayerVessel::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Red,
	"no PlayerController !"
	);
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer == nullptr) {
		GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Red,
	"no LocalPlayer !"
	);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if(Subsystem == nullptr) {
		GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Red,
	"no SubSystem!"
	);
		return;
	}
	
	Subsystem->AddMappingContext(InputMappingContext,0);

	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Green,
	"IMC SET !"
	);
}


void APlayerVessel::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	SetupMappingContextIntoController();

	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
	
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr) return;
	
	BindInputMoveAxisAndActions(EnhancedInputComponent);
}

void APlayerVessel::BindInputMoveAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(InputMappingContext == nullptr) return;
	if(EnhancedInputComponent == nullptr) return;

	if(MoveXInputAction)
	{
		EnhancedInputComponent->BindAction
		(
			MoveXInputAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerVessel::OnXMove
		);
	}
	if(MoveYInputAction)
	{
		EnhancedInputComponent->BindAction
		(
			MoveYInputAction,
			ETriggerEvent::Triggered,
			this,
			&APlayerVessel::OnYMove
		);
	}
	
}


void APlayerVessel::OnXMove(const FInputActionValue& InputActionValue)
{

	float Movement = InputActionValue.Get<float>();
	
	PositionOffset.X += Movement;

	PositionOffset.X = FMath::Clamp(PositionOffset.X,0,MaxXYDistance.X);
}

void APlayerVessel::OnYMove(const FInputActionValue& InputActionValue)
{
	float Movement = InputActionValue.Get<float>();

	PositionOffset.Y += Movement;

	PositionOffset.Y = FMath::Clamp(PositionOffset.Y,0,MaxXYDistance.Y);

}

#pragma endregion



