// FpsCoreCharacters.Build.cs
using UnrealBuildTool;

public class FpsCoreInteractables : ModuleRules
{
    public FpsCoreInteractables(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine"
        });
    }
}