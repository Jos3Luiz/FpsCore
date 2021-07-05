// FpsCoreCharacters.Build.cs
using UnrealBuildTool;

public class FpsCoreCharacters : ModuleRules
{
    public FpsCoreCharacters(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "NavigationSystem",
            "AIModule",
            "FpsCoreWeapons",
            "FpsCoreComponents",
            "FpsCore"
        });
    }
}