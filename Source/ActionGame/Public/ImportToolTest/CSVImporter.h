

#pragma once

#include "CoreMinimal.h"

#include "CSVImporter.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UCSVImporter : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	public:
	UFUNCTION(BlueprintCallable, CallInEditor,Category="CSV Import")
	static void ImportItemsCSV(const FString& CSVFilePath, const FString& DestinationPath);
};
