#include "ImportToolTest/CSVImporter.h"

#include "ObjectTools.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ImportToolTest/ItemDataAsset.h"
#include "UObject/SavePackage.h"


void UCSVImporter::ImportItemsCSV(const FString& CSVFilePath, const FString& DestinationPath)
{
	FString CSVContent;
	if (!FFileHelper::LoadFileToString(CSVContent, *CSVFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load CSV file: %s"), *CSVFilePath);
		return;
	}

	TArray<FString> Lines;
	CSVContent.ParseIntoArrayLines(Lines);
	if (Lines.Num() <= 1)
	{
		UE_LOG(LogTemp, Error, TEXT("CSV file is empty"));
		return;
	}
	/*The AssetRegistry is Unreal’s database of all assets in your project (materials, blueprints, DataAssets, etc.).
	LoadModuleChecked ensures the module loads successfully; if it doesn’t, the program will crash with a helpful error (useful for debugging).*/
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
	/*Adds a filter so only assets of type UItemDataAsset are returned.*/
	FARFilter Filter;
	Filter.ClassPaths.Add(UItemDataAsset::StaticClass()->GetClassPathName());
	
	/*Adds another filter to only search inside the given package path.Tells the filter to also search subfolders under DestinationPath*/
	Filter.PackagePaths.Add(*DestinationPath);
	Filter.bRecursivePaths = true;

	/*Executes the search using the filter and stores the results in ExistingAssets*/
	TArray<FAssetData> ExistingAssets;
	AssetRegistryModule.Get().GetAssets(Filter, ExistingAssets);

	TMap<FString, FAssetData> ExistingAssetsMap;
	for (const FAssetData& AssetData : ExistingAssets)
	{
		ExistingAssetsMap.Add(AssetData.AssetName.ToString(), AssetData);
	}


	TSet<FString> CSVIDs;
	for (int32 i = 1; i < Lines.Num(); i++)
	{
		TArray<FString> Values;
		Lines[i].ParseIntoArray(Values, TEXT(","), true);

		FString ID = Values[0];
		FString Name = Values[1];
		int32 Damage = FCString::Atoi(*Values[2]);
		float Weight = FCString::Atof(*Values[3]);
		FString Description = Values[4];

		CSVIDs.Add(ID);

		FString PackageName = DestinationPath + "/" + ID;
		UPackage* Package = CreatePackage(*PackageName);

		UItemDataAsset* NewAsset = nullptr;
		if (ExistingAssetsMap.Contains(ID))
		{
			NewAsset = Cast<UItemDataAsset>(
				StaticLoadObject(UItemDataAsset::StaticClass(), nullptr, *(PackageName + "." + ID)));
		}
		if (!NewAsset)
		{
			NewAsset = NewObject<UItemDataAsset>(Package, *ID, RF_Public | RF_Standalone);
		}
		NewAsset->ID = FName(*ID);
		NewAsset->Name = Name;
		NewAsset->Damage = Damage;
		NewAsset->Weight = Weight;
		NewAsset->Description = Description;
		
		FAssetRegistryModule::AssetCreated(NewAsset);

		/*Converts Unreal’s long package name into an absolute file system path with the right extension.*/
		FString FilePath = FPackageName::LongPackageNameToFilename(PackageName,
		                                                           FPackageName::GetAssetPackageExtension());
		FSavePackageArgs SaveArgs;
		/*These flags mean the asset is visible in the editor (Public) and stored independently (Standalone), so it won’t be garbage-collected.*/
		SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
		/*Ensures consistent endianness in the file, making it portable across platforms.*/
		SaveArgs.bForceByteSwapping = true;
		/*Provides an error output device (so if saving fails, Unreal will log a message).*/
		SaveArgs.Error = GError;

		/*Actually saves the package (which contains NewAsset) to disk*/
		bool bSaved = UPackage::SavePackage(Package, NewAsset, *FilePath, SaveArgs);
		if (bSaved)
		{
			UE_LOG(LogTemp, Log, TEXT("Saved asset: %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save asset: %s"), *FilePath);
		}
	}


	for (const auto& Pair : ExistingAssetsMap)
	{
		if (!CSVIDs.Contains(Pair.Key))
		{
			/*GetAsset() actually loads the asset (a real UObject) from disk into memory*/
			UObject* Asset = Pair.Value.GetAsset();
			if (Asset)
			{
				/*Asset->GetOutermost() returns the outermost UObject, which is always the UPackage that holds the asset.*/
				const TArray<UObject*> PackagesToDelete = {Cast<UPackage>(Asset->GetOutermost())};
				ObjectTools::DeleteObjects(PackagesToDelete, false);
				UE_LOG(LogTemp, Log, TEXT("Deleted asset: %s"), *Pair.Key);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("CSV import finished. Created/Updated: %d assets, Deleted: %d assets."),
	       CSVIDs.Num(), ExistingAssetsMap.Num() - CSVIDs.Num());
}
