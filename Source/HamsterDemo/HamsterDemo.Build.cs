// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class HamsterDemo : ModuleRules
{
	public HamsterDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
        PublicDependencyModuleNames.AddRange(new string[] { "NavigationSystem", "AIModule", "GamePlayTasks" });
    }	
}
