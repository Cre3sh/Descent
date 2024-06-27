// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DescentTheGameTarget : TargetRules
{
	public DescentTheGameTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        ExtraModuleNames.AddRange(new string[] {"DescentTheGame"});
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
	}
}