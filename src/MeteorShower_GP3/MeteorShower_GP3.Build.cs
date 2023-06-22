// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MeteorShower_GP3 : ModuleRules
{
	public MeteorShower_GP3(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		//	add core stuff
		PublicDependencyModuleNames.AddRange(new[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "CableComponent" });

		// add JUICE modules
		PublicDependencyModuleNames.AddRange(new[]
			{ "Niagara" });
		
		// add UI modules
		PublicDependencyModuleNames.AddRange(new[]
			{ "Slate", "SlateCore", "UMG" });
		
		// add AI modules
		PublicDependencyModuleNames.AddRange(new[]
			{ "NavigationSystem", "AIModule" });
		
		//	tasks & threading
		PublicDependencyModuleNames.AddRange(new[]
			{ "GameplayTasks", "GameplayCameras" });

		PrivateDependencyModuleNames.AddRange(new string[]
			{ "CableComponent" });
	}
}