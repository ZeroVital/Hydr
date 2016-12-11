using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class HydrLoadingScreen : ModuleRules
{
    public HydrLoadingScreen(TargetInfo Target)
	{
		PrivateIncludePaths.Add("../../Hydr/Source/HydrLoadingScreen/Private");

        PublicDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"Engine"
			}
		);

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"MoviePlayer",
				"Slate",
				"SlateCore",
				"InputCore"
			}
		);
	}
}
