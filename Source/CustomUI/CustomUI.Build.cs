using UnrealBuildTool;

public class CustomUI : ModuleRules
{
    public CustomUI(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"EnhancedInput",
			"InputCore",    // Required for EKeys
			"ToolMenus",    // Required for ToolMenus API
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore", 
                "Sacha_Epry_Rendu"
            }
        );
    }
}