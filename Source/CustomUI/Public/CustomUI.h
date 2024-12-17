#pragma once

#include "Modules/ModuleManager.h"
#include "Internationalization/Text.h"
#include "InputCoreTypes.h"

class USplineFollower;

class FCustomUIModule : public IModuleInterface
{
public:
 
    void StartupModule() override;
    
    void OnWorldCreated(UWorld* NewWorld, const UWorld::InitializationValues  IVS);

    
    
    void ShutdownModule() override;

    void FindSplineFollowers();

    UWorld* World;
    USplineFollower* PlayerSplineFollower;

    void SetSpeedToValue(float value);
private:

    //this is to create The buttons ok
    void RegisterMenuExtensions();

protected:
};
 
IMPLEMENT_MODULE(FCustomUIModule, CustomUI)