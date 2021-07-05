// FpsCoreCharacters.Build.cs
using UnrealBuildTool;

public class FpsCoreComponents : ModuleRules
{
    public FpsCoreComponents(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "FpsCoreWeapons",
            "FpsCore"
        });
    }
}