// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ElevenBitAssignement : ModuleRules
{
	public ElevenBitAssignement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NavigationSystem" });
	}
}
