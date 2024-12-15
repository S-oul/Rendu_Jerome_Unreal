#pragma once

#include "Modules/ModuleManager.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Internationalization/Text.h"
#include "InputCoreTypes.h"
#include "Widgets/Input/SSpinBox.h"
#include "ToolMenus.h"

class FCustomUIModule : public IModuleInterface
{
public:
 
    void StartupModule() override
    {
        // Register a function to be called when menu system is initialized
        UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(
            this, &FCustomUIModule::RegisterMenuExtensions));
    }
 
    void ShutdownModule() override
    {
        // Unregister the startup function
        UToolMenus::UnRegisterStartupCallback(this);
 
        // Unregister all our menu extensions
        UToolMenus::UnregisterOwner(this);
    }
 
private:
 
    void RegisterMenuExtensions()
    {
        FToolMenuOwnerScoped OwnerScoped(this);

        UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu(
            "LevelEditor.LevelEditorToolBar.PlayToolBar");
        FToolMenuSection& ToolbarSection = ToolbarMenu->FindOrAddSection("GameVariables");

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
};
 
IMPLEMENT_MODULE(FCustomUIModule, CustomUI)