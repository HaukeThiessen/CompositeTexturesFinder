#include "CompositeTexturesFinder.h"
#include "DataValidation/Public/DataValidationSettings.h"
#include "EditorScriptingUtilities/Public/EditorAssetLibrary.h"
#include "Misc/DataValidation.h"
#include "CompositeTexturesFinderSettings.h"

IMPLEMENT_MODULE(FCompositeTexturesFinderModule, CompositeTexturesFinder)

bool UCompositeTexturesValidator::CanValidateAsset_Implementation(
	const FAssetData& InAssetData, UObject* InObject, FDataValidationContext& InContext) const
{
	UTexture2D* Texture = Cast<UTexture2D>(InObject);
	if (!Texture)
	{
		return false;
	}

	// If a composite texture is set, don't interfere
	if (Texture->GetCompositeTexture())
	{
		return false;
	}

	const TArray<FCompositeTexturesPair> Pairs = GetDefault<UCompositeTexturesFinderSettings>()->CompositeTexturePairs;
	for (FCompositeTexturesPair Pair : Pairs)
	{
		if (InObject->GetName().EndsWith(Pair.RougnessMapSuffix))
		{
			return true;
		}
	}
	return false;
}

EDataValidationResult UCompositeTexturesValidator::ValidateLoadedAsset_Implementation(
	const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& Context)
{
	const TArray<FCompositeTexturesPair> Pairs = GetDefault<UCompositeTexturesFinderSettings>()->CompositeTexturePairs;
	for (FCompositeTexturesPair Pair : Pairs)
	{
		const FString NormalMapName =
			InAssetData.PackageName.ToString().Replace(*Pair.RougnessMapSuffix, *Pair.NormalMapSuffix);

		if (!UEditorAssetLibrary::DoesAssetExist(NormalMapName))
		{
			continue;
		}

		UTexture2D* NormalMap = Cast<UTexture2D>(UEditorAssetLibrary::LoadAsset(NormalMapName));
		if (!NormalMap)
		{
			AssetPasses(InAsset);
			return EDataValidationResult::Valid;
		}

		UTexture2D* Texture = Cast<UTexture2D>(InAsset);
		UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>();
		const bool WasEditorOpen = AssetEditorSubsystem->FindEditorForAsset(InAsset, false) != NULL;
		if (WasEditorOpen)
		{
			AssetEditorSubsystem->CloseAllEditorsForAsset(InAsset);
		}
		InAsset->Modify();

		// The actual change, everything else is there to make this behave nice
		Texture->SetCompositeTexture(NormalMap);
		Texture->CompositeTextureMode = ECompositeTextureMode::CTM_NormalRoughnessToGreen;

		InAsset->PostEditChange();

		bool SavedSuccessfully = false;
		if (GetDefault<UDataValidationSettings>()->bValidateOnSave)
		{
			UDataValidationSettings* Settings = GetMutableDefault<UDataValidationSettings>();
			Settings->bValidateOnSave = false;
			SavedSuccessfully = UEditorAssetLibrary::SaveLoadedAsset(InAsset, true);
			Settings->bValidateOnSave = true;
		}
		else
		{
			SavedSuccessfully = UEditorAssetLibrary::SaveLoadedAsset(InAsset, true);
		}

		FText Message = FText::FromString(
			"Set " + NormalMap->GetName() + " as composite texture for " + InAssetData.AssetName.ToString());
		if (!SavedSuccessfully)
		{
			Message = FText::FromString(Message.ToString() + ", but failed to save the changed asset");
		}
		Context.AddMessage(EMessageSeverity::Info, Message);

		if (WasEditorOpen)
		{
			AssetEditorSubsystem->OpenEditorForAsset(InAsset);
		}
		break;
	}
	AssetPasses(InAsset);
	return EDataValidationResult::Valid;
}