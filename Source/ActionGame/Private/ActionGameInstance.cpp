// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGameInstance.h"

#include "Library/StructLibrary.h"

TSoftObjectPtr<UWorld> UActionGameInstance::GetGameLevelByTag(FGameplayTag InTag)
{
	for (const FActionGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid()) continue;
		if (GameLevelSet.LevelTag==InTag)
		{
			return GameLevelSet.Level;
		}	
	}
	return TSoftObjectPtr<UWorld>();
}
