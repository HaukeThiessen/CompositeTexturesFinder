using UnrealBuildTool;

public class CompositeTexturesFinder : ModuleRules
{
	public CompositeTexturesFinder(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"DeveloperSettings"
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"UnrealEd",
				"CoreUObject",
				"Engine",
				"DataValidation",
				"EditorScriptingUtilities",
				"DeveloperToolSettings"
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{

			}
			);
	}
}
