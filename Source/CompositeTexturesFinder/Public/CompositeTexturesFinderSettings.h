#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "CompositeTexturesFinderSettings.generated.h"

USTRUCT(BlueprintType)
struct FCompositeTexturesPair
{
	GENERATED_BODY()

	// The suffix to identify the roughness map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompositeTextures")
	FString RougnessMapSuffix;

	// The suffix to identify the normal map
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompositeTextures")
	FString NormalMapSuffix;

	// The composite mode to set when a pair is found
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TextureGroups")
	TEnumAsByte<ECompositeTextureMode> CompositeMode = ECompositeTextureMode::CTM_NormalRoughnessToGreen;
};

UCLASS(config = Game, defaultconfig, meta = (DisplayName = "Composite Maps Finder"))
class UCompositeTexturesFinderSettings : public UDeveloperSettings
{
	GENERATED_BODY()

  public:
	virtual FText GetSectionDescription() const override
	{
		return INVTEXT("Configures the Composite Textures Finder");
	}
	virtual FName GetCategoryName() const override
	{
		return FName("Plugins");
	}

	// Pairings of texture suffixes to apply composite textures
	UPROPERTY(Config, EditAnywhere, Category = "Textures")
	TArray<FCompositeTexturesPair> CompositeTexturePairs;
};
