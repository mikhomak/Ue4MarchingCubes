// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
using System;

public class MarchingCubes : ModuleRules
{
	public MarchingCubes(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		LoadLib(Target, "../../ThirdParty/LibNoise", "LIB_NOISE", "libnoise");
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "RuntimeMeshComponent" });
	}

	public bool LoadLib(ReadOnlyTargetRules Target, string libPath, string libName, string libFileName)
	{
		bool isLibrarySupported = false;

		if (Target.Platform == UnrealTargetPlatform.Win64 || Target.Platform == UnrealTargetPlatform.Win32) {
			isLibrarySupported = true;
			string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
			string LibrariesPath = Path.Combine(ModuleDirectory, libPath, "Libraries");

			PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, libFileName + "." + PlatformString + ".lib"));
			PublicIncludePaths.Add(Path.Combine(ModuleDirectory, libPath, "Includes"));
			 Console.WriteLine(Path.Combine(ModuleDirectory, libPath, "Includes"));
		}
		PublicDefinitions.Add(string.Format("WITH_" + libName + "_BINDING={0}", isLibrarySupported ? 1 : 0));
		return isLibrarySupported;
	}
}
