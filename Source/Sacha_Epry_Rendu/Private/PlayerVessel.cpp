// Fill out your copyright notice in the Description page of Project Settings.


#include "Sacha_Epry_Rendu/Public/PlayerVessel.h"

#include "AssetDefinition.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LaserProjectile.h"


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

	PositionOffset = FVector2D(0);
	
	SplineFollowerComponent = Cast<USplineFollower>(GetComponentByClass(USplineFollower::StaticClass()));
	if(SplineFollowerComponent == nullptr)
	{
		GEngine->AddOnScreenDebugMessage
		(
		-1,
		10,
		FColor::Purple,
		"NO SPLINE FOLLOWER"
		);
		return;
	}

	TArray<UStaticMeshComponent*> Allmesh;
	GetComponents<UStaticMeshComponent>(Allmesh);
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

	SplineFollowerComponent->SetPlayerInputOffset(PositionOffset, XMoveHeat, YMoveHeat);
	
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
	
	BindInputAndActions(EnhancedInputComponent);
}

void APlayerVessel::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent)
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
		EnhancedInputComponent->BindAction
		(
			MoveXInputAction,
			ETriggerEvent::Completed,
			this,
			&APlayerVessel::OnReleaseX
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
		EnhancedInputComponent->BindAction
		(
			MoveYInputAction,
			ETriggerEvent::Completed,
			this,
			&APlayerVessel::OnReleaseY
		);
	}

	if(ShootAction)
	{
		EnhancedInputComponent->BindAction
		(
			ShootAction,
			ETriggerEvent::Started,
			this,
			&APlayerVessel::OnShoot
		);
	}
	
}


void APlayerVessel::OnXMove(const FInputActionValue& InputActionValue)
{
	float Movement = InputActionValue.Get<float>();

	XMoveHeat = Movement;

	PositionOffset.X += Movement * MovementSpeed;
	PositionOffset.X = FMath::Clamp(PositionOffset.X,-MaxXYDistance.X,MaxXYDistance.X);
}

void APlayerVessel::OnYMove(const FInputActionValue& InputActionValue)
{
	float Movement = InputActionValue.Get<float>();

	YMoveHeat = Movement;
	
	PositionOffset.Y += Movement * MovementSpeed;
	PositionOffset.Y = FMath::Clamp(PositionOffset.Y,-MaxXYDistance.Y,MaxXYDistance.Y);
}

void APlayerVessel::OnReleaseY()
{
	YMoveHeat = 0;
}
void APlayerVessel::OnReleaseX()
{
	XMoveHeat = 0;
}


void APlayerVessel::OnShoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow,
		FString::Printf(TEXT("HeatValues: X: %f Y: %f"), XMoveHeat, YMoveHeat));

	FActorSpawnParameters SpawnParameters;

	
	FRotator Rotator = this->GetActorForwardVector().Rotation() + FRotator(YMoveHeat*15, XMoveHeat*25, 0);
	FVector loc = ShipMesh->GetComponentLocation();	
	if(LaserActor) {
		AActor* laser = GetWorld()->SpawnActor(LaserActor, &loc, &Rotator, SpawnParameters);
		UE_LOG(LogTemp,Display,TEXT("Laser Shoot ")); 

	}
}

#pragma endregion



