// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EPTA_FPP_KIBERSRYNKEditorTarget : TargetRules
{
	public EPTA_FPP_KIBERSRYNKEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V6;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
		ExtraModuleNames.Add("EPTA_FPP_KIBERSRYNK");
	}
}
