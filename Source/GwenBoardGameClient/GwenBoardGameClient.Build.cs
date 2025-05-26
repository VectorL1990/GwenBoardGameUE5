// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class GwenBoardGameClient : ModuleRules
{
	public GwenBoardGameClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("Game");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "KBEnginePlugins" });

		PrivateDependencyModuleNames.AddRange(new string[] {  "Networking", "Slate", "SlateCore" });

        PublicDependencyModuleNames.AddRange(new string[] { "HTTP" });

        PublicDependencyModuleNames.AddRange(new string[] { "Json", "JsonUtilities" });

        PublicIncludePaths.Add(Path.Combine("Game"));

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
