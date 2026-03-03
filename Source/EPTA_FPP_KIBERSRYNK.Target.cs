// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EPTA_FPP_KIBERSRYNKTarget : TargetRules
{
	public EPTA_FPP_KIBERSRYNKTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("EPTA_FPP_KIBERSRYNK");
	}
}
