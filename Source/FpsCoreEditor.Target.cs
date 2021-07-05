// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class FpsCoreEditorTarget : TargetRules
{
	public FpsCoreEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.AddRange( new string[] { "FpsCore","FpsCoreAttachments","FpsCoreCharacters","FpsCoreComponents","FpsCoreInteractables","FpsCoreWeapons" } );
	}
}
