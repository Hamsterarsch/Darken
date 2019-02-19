// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class Limes : ModuleRules
{
	public Limes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule" });
                
        PublicAdditionalLibraries.Add( Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/PolarMath.lib")) );
        PublicIncludePaths.Add( Path.Combine(ModuleDirectory, "../../PolarMath/hdr/") );

        System.Console.WriteLine(Path.Combine(ModuleDirectory, "../../PolarMath/hdr/"));
        
		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
