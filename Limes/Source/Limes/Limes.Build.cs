// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class Limes : ModuleRules
{
	public Limes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG"});

        if (Target.bBuildEditor)
        {
            PublicDependencyModuleNames.Add("PropertyEditor");
        }

		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		PrivateDependencyModuleNames.AddRange(new string[] { "GameAnalytics" });
		PrivateIncludePathModuleNames.AddRange(new string[] { "GameAnalytics"});
		
        //Polar math lib
        PublicAdditionalLibraries.Add( Path.GetFullPath(Path.Combine(ModuleDirectory, "../../Binaries/Win64/PolarMath.lib")) );
        PublicIncludePaths.Add( Path.Combine(ModuleDirectory, "../../PolarMath/hdr/") );

        System.Console.WriteLine
        (
            "Adding static library under path:\n\t\t" 
            + Path.Combine(ModuleDirectory, "../../PolarMath/hdr/") 
            + "\n\t\tto Limes dependencies"
        );
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
