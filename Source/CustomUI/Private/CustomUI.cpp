#include "CustomUI.h"

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

	
	RegisterMenuExtensions();
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

	if(!PlayerVessel) UE_LOG(LogTemp, Error, TEXT("PlayerVessel is SHIT"))
	else UE_LOG(LogTemp, Display , TEXT("PlayerVessel is SUCCESS : %s"), *PlayerVessel->GetActorLabel());
	
	USplineFollower* SplineFollower = PlayerVessel->FindComponentByClass<USplineFollower>();
	
	if(!SplineFollower) UE_LOG(LogTemp, Error, TEXT("PlayerSplineFollower is ULTIME SHIT"))
	else UE_LOG(LogTemp, Display , TEXT("PlayerSplineFollower is SUCCESS : %s"), *SplineFollower->GetName());

	this->PlayerSplineFollower = SplineFollower;	
}
void FCustomUIModule::SetSpeedToValue(float value)
{
	this->PlayerSplineFollower->FollowSpeed = value;
	
}
void FCustomUIModule::RegisterMenuExtensions()
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
	
	ToolbarSection.AddEntry(FToolMenuEntry::InitWidget(
		"FloatInputBox",
		SNew(SNumericEntryBox<float>)
			.AllowSpin(true)
			.MinSliderValue(1).MaxSliderValue(500).Value(300)
			.SliderExponent(.75f)
			.SupportDynamicSliderMaxValue(true)
			.Value_Lambda([]() { return 300.f; }) 
			.OnValueChanged_Lambda([this](float NewValue) { // So fckin far fetched
				if(!PlayerSplineFollower) // if isnt set
				{
					this->FindSplineFollowers();
				}
				if(this->PlayerSplineFollower) // if cast failed
				{
					this->SetSpeedToValue(NewValue);
					UE_LOG(LogTemp, Log, TEXT("New value: %f"), NewValue);
				}else UE_LOG(LogTemp, Log, TEXT("CAST IS SHIT"));

				
			}),
		FText::FromString("Float Input"),
		true
		 )
	);
}




