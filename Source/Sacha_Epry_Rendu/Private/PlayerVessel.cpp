// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/PlayerVessel.h"
#include "EnhancedInputSubsystems.h"


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
	
}

// Called every frame
void APlayerVessel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerVessel::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerVessel::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr) return;

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	if(Subsystem == nullptr) return;
	
	Subsystem->AddMappingContext(InputMappingContext,0);

	GEngine->AddOnScreenDebugMessage
	(
	-1,
	10.f,
	FColor::Green,
	"IMC SET !"
	);

}

