// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LibCjson : ModuleRules
{
	public LibCjson(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core" });

		PrivateDependencyModuleNames.AddRange(new string[] { "Lua" });

		//bEnableShadowVariableWarnings = false;
		bEnableUndefinedIdentifierWarnings = false;

		PublicDefinitions.Add("WITH_LibCjson=1");
	}
}
