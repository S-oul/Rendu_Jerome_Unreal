#include "CustomUI.h"

#include "SplineFollower.h"
#include "ToolMenus.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/Input/SNumericEntryBox.h"

#define LOCTEXT_NAMESPACE "FCustomUIModule"

void FCustomUIModule::StartupModule()
{


	/// Aled les delegates j'en peux plus
	FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &FCustomUIModule::OnWorldCreated);

	//UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FCustomUIModule::RegisterMenuExtensions));


	//FWorldDelegates::FWorldInitializationEvent::FDelegate OnWorldCreatedDelegate;
	//OnWorldCreatedDelegate = FWorldDelegates::FWorldInitializationEvent::FDelegate::CreateRaw(this, &FCustomUIModule::OnWorldCreated);
	//FDelegateHandle OnWorldCreatedDelegateHandle = FWorldDelegates::OnPostWorldInitialization.Add(OnWorldCreatedDelegate);

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

TArray<USplineFollower*> FCustomUIModule::FindSplineFollowers()
{
	TArray<AActor*> AllActor;
	UGameplayStatics::GetAllActorsOfClass(this->World, USplineFollower::StaticClass(), AllActor);

	UE_LOG(LogTemp, Warning, TEXT("Number of Spline : %d"), AllActor.Num());
	TArray<USplineFollower*> FoundedSplineFollowers;
	int i =0;
	for (AActor* Actor : AllActor)
	{
		FoundedSplineFollowers[i++] = Cast<USplineFollower>(Actor);
	}

	
	this->AllSplineFollowers = FoundedSplineFollowers;
	return FoundedSplineFollowers;
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
			.MinValue(0.f)       
			.MaxValue(500.f)     
			.Value_Lambda([]() { return 300.f; }) 
			.OnValueChanged_Lambda([](float NewValue) {
				UE_LOG(LogTemp, Log, TEXT("New value: %f"), NewValue);
			}),
		FText::FromString("Float Input"),
		true
		 )
	);
}
