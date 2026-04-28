#pragma once

#include "Modules/ModuleManager.h"
#include "CoreMinimal.h"
#include "DataValidation/Public/EditorValidatorBase.h"
#include "CompositeTexturesFinder.generated.h"

class FCompositeTexturesFinderModule : public IModuleInterface
{
};

UCLASS()
class UCompositeTexturesValidator : public UEditorValidatorBase
{
	GENERATED_BODY()

  protected:
	virtual bool CanValidateAsset_Implementation(
		const FAssetData& inAssetData, UObject* inObject, FDataValidationContext& inContext) const override;

	virtual EDataValidationResult ValidateLoadedAsset_Implementation(
		const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& Context) override;
};