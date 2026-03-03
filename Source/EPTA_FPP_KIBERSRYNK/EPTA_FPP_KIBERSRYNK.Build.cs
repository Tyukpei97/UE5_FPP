// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EPTA_FPP_KIBERSRYNK : ModuleRules
{
	public EPTA_FPP_KIBERSRYNK(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"EPTA_FPP_KIBERSRYNK",
			"EPTA_FPP_KIBERSRYNK/Variant_Horror",
			"EPTA_FPP_KIBERSRYNK/Variant_Horror/UI",
			"EPTA_FPP_KIBERSRYNK/Variant_Shooter",
			"EPTA_FPP_KIBERSRYNK/Variant_Shooter/AI",
			"EPTA_FPP_KIBERSRYNK/Variant_Shooter/UI",
			"EPTA_FPP_KIBERSRYNK/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
