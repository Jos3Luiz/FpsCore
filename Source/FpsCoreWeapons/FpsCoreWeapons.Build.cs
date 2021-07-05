// FpsCoreCharacters.Build.cs
using UnrealBuildTool;

public class FpsCoreWeapons : ModuleRules
{
    public FpsCoreWeapons(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "Engine",
            "CoreUObject",
            "FpsCoreInteractables",
            "FpsCoreAttachments",
            "Niagara",
            "FpsCore"
        });
    }
}