// FpsCoreCharacters.Build.cs
using UnrealBuildTool;

public class FpsCoreAttachments : ModuleRules
{
    public FpsCoreAttachments(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine"
        });
    }
}