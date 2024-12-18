#include "CustomUI.h"

#include "Editor.h"
#include "PlayerVessel.h"
#include "SplineFollower.h"
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "FCustomUIModule"

void FCustomUIModule::StartupModule()
{



	//UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCustomUIModule::RegisterMenuExtensions));


	//FWorldDelegates::FWorldInitializationEvent::FDelegate OnWorldCreatedDelegate;
	//OnWorldCreatedDelegate = FWorldDelegates::FWorldInitializationEvent::FDelegate::CreateRaw(this, &FCustomUIModule::OnWorldCreated);
	//FDelegateHandle OnWorldCreatedDelegateHandle = FWorldDelegates::OnPostWorldInitialization.Add(OnWorldCreatedDelegate);

	
	/// Aled les delegates j'en peux plus
	FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &FCustomUIModule::OnWorldCreated);
	FEditorDelegates::BeginPIE.AddRaw(this, &FCustomUIModule::OnBeginPIE);
	FEditorDelegates::EndPIE.AddRaw(this, &FCustomUIModule::OnEndPIE);
	
	RegisterMenuExtensions(false);

	
}
void FCustomUIModule::OnBeginPIE(const bool bIsSimulating)
{
	UE_LOG(LogTemp, Log, TEXT("Begin Play detected in Plugin!"));
	RegisterMenuExtensions(true);
}
void FCustomUIModule::OnEndPIE(bool bIsSimulating)
{
	UE_LOG(LogTemp, Log, TEXT("Begin Play stopped detected in Plugin!"));
	RegisterMenuExtensions(false);
}


void FCustomUIModule::OnWorldCreated(UWorld* NewWorld, const UWorld::InitializationValues  IVS)
{
	this->World = NewWorld;
	
    UE_LOG(LogTemp, Warning, TEXT("OnWorldCreated was called: %s"), *World->GetName());

	FindSplineFollowers();
}

void FCustomUIModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
	
	// Unregister the startup function
	UToolMenus::UnRegisterStartupCallback(this);
 
	// Unregister all our menu extensions
	UToolMenus::UnregisterOwner(this);
	
}

void FCustomUIModule::FindSplineFollowers()
{
	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsOfClass(this->World, APlayerVessel::StaticClass(), AllActor);

	UE_LOG(LogTemp, Warning, TEXT("in %s, Number of SplineFollower : %d"),*World->GetName(), AllActor.Num());
	if(AllActor.Num() == 0) return;

	AActor* PlayerActor = AllActor[0];
	UE_LOG(LogTemp, Warning, TEXT("Player is %s"),*PlayerActor->GetActorLabel());
	
	APlayerVessel* PlayerVessel = Cast<APlayerVessel>(PlayerActor); // when I try to make shortcut like : this->PlayerSplineFollower = AllActor[0]->FindComponentByClass<USplineFollower>(); it breaks whyyyyyyyyyyyyyyyyyy uneral ?????????????

	if(!PlayerVessel) UE_LOG(LogTemp, Error, TEXT("Cast PlayerVessel is failed"))
	else UE_LOG(LogTemp, Display , TEXT("Cast PlayerVessel is success : %s"), *PlayerVessel->GetActorLabel());
	
	USplineFollower* SplineFollower = PlayerVessel->FindComponentByClass<USplineFollower>();
	
	if(!SplineFollower) UE_LOG(LogTemp, Error, TEXT("Cast PlayerSplineFollower is failed"))
	else UE_LOG(LogTemp, Display , TEXT("Cast PlayerSplineFollower is success : %s"), *SplineFollower->GetName());

	this->PlayerSplineFollower = SplineFollower;	
}
void FCustomUIModule::SetSpeedToValue(float value)
{
	this->PlayerSplineFollower->FollowSpeed = value;
	
}
void FCustomUIModule::RegisterMenuExtensions(bool ShoudButtonWork)
{
	FToolMenuOwnerScoped OwnerScoped(this);

	UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(
		"LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("GameVariables");

	ToolbarSection.AddEntry(FToolMenuEntry::InitToolBarButton(
			   TEXT("MyCustomButtonName"),
			   FExecuteAction::CreateLambda([this]()
			   {
			   		FindSplineFollowers();
			   }),
			   INVTEXT("Find SplineFollowers in Current World"),
			   INVTEXT("Find SplineFollowers in Current World")
		   ));

	
	FToolMenuEntry Slider = ToolbarSection.AddEntry(FToolMenuEntry::InitWidget(
		"FloatInputBox",
		SNew(SNumericEntryBox<float>)
			.AllowSpin(true)
			.MinSliderValue(1).MaxSliderValue(500).Value(300)
			.SliderExponent(.75f)
			.IsEnabled_Lambda([ShoudButtonWork]()
				{
					return ShoudButtonWork; // le POULET
				})
			.Value_Lambda([this]() { return SliderValue; }) // these two are 
			.OnValueChanged_Lambda([this](float NewValue) { // So fckin far fetched j'ai mis tellement longtemps à trouver
				/*if (!GIsPlayInEditorWorld)
				{
					UE_LOG(LogTemp, Warning, TEXT("Cannot adjust speed: You must be in Play mode."));
					return;
				}*/
				if (!PlayerSplineFollower)
				{
					UE_LOG(LogTemp, Log, TEXT("PlayerSplineFollower not set. Attempt to find spline follower"));
					FindSplineFollowers();
				}
				if (PlayerSplineFollower)
				{
					SetSpeedToValue(NewValue);
					this->SliderValue = static_cast<int>(NewValue);
					UE_LOG(LogTemp, Log, TEXT("Speed updated successfully. New value: %f"), NewValue);
				}else UE_LOG(LogTemp, Error, TEXT("Failed to find or cast PlayerSplineFollower. Speed adjustment failed."));
			}),
		FText::FromString("Float Input"),
		true
		 )
	);
}