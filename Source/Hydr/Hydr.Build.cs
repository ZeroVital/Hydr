// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class Hydr : ModuleRules
{
	public Hydr(TargetInfo Target)
	{
        MinFilesUsingPrecompiledHeaderOverride = 1;
        bFasterWithoutUnity = true;
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore",
                //"AIModule",
                //"GameplayTasks",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "HydrLoadingScreen"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Slate",
                "SlateCore",
            }
        );


        PrivateIncludePaths.AddRange(
            new string[] {
                "Hydr/Private/UI/Menu",
                "Hydr/Private/UI/Style",
                "Hydr/Private/UI/Widgets",
            }
        );
    }
}
