using UnrealBuildTool;
using System.IO;

public class CombatSystemCpp : ModuleRules
{
    public CombatSystemCpp(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

       
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "01_Character"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "02_Weapon"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "03_Interface"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "04_Enemy"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "GameMode"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Notify"));
    }
}
