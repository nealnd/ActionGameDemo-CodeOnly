// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartUpDataBase.h"
#include "Library/StructLibrary.h"
#include "DataAsset_HeroStartUpData.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_HeroStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly,Category="StartUpData",meta=(TitleProperty="InputTag"))
	TArray<FActionHeroAbilitySet>HeroStartUpAbilitySets;
	
public:
	virtual void GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InActionASCToGive,int32 ApplyLevel=1) override;
};


