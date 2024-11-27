// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/PlayerVessel.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
APlayerVessel::APlayerVessel()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//INIT
	// Create and initialize the spring arm
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent); // Attach to root

	// Create and initialize the camera
	MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	MainCamera->SetupAttachment(SpringArmComponent); // Attach to spring arm
	
	if (MainCamera == nullptr) 	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Red,
	"no MAIN CAM !"
	);


	if (SpringArmComponent == nullptr) 	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Red,
	"no SPRINARM !"
	);

	MainCamera->bv
}

// Called when the game starts or when spawned
void APlayerVessel::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void APlayerVessel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	SetActorRotation(MainCamera->GetRelativeRotation());
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

	GEngine->AddOnScreenDebugMessage
	(
	-1,
	2.1f,
	FColor::Yellow,
    FString::Printf(TEXT("X: %f"), Movement)
	);
	
}

void APlayerVessel::OnYMove(const FInputActionValue& InputActionValue)
{
	float Movement = InputActionValue.Get<float>();

	GEngine->AddOnScreenDebugMessage
	(
	-1,
	2.1f,
	FColor::Yellow,
	FString::Printf(TEXT("Y: %f"), Movement)
	);
	
}

#pragma endregion



