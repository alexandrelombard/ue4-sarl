// Copyright (c) 2018 Isara Technologies. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class UE4SARLRuntime: ModuleRules
	{
		public UE4SARLRuntime(ReadOnlyTargetRules Target) : base(Target)
		{
			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
			
			PublicIncludePaths.AddRange(
				new string[] {
					// ... add public include paths required here ...
				}
				);

			PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
				}
				);

			PublicDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
                    "Sockets",
                    "Networking"
                    // ... add other public dependencies that you statically link with here ...
                }
                );

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{                    
					"libprotobuf"
				}
				);

			DynamicallyLoadedModuleNames.AddRange(
				new string[]
				{
					// ... add any modules that your module loads dynamically here ...
				}
				);
				
			PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings",
                "Launch"
            }
            );

		}
	}
}
