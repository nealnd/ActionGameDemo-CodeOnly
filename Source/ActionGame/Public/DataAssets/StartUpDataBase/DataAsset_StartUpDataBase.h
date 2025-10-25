// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Library/StructLibrary.h"
#include "DataAsset_StartUpDataBase.generated.h"


class UGameplayEffect;
class UActionAbilitySystemComponent;
class UActionGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_StartUpDataBase : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InActionASCToGive,int32 ApplyLevel=1) ;

	
	const TMap<int32,FActionHitReactionAbilityContainer>& GetHitReactionAbilities()  {return HitReactionAbilities;}

protected:
	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UActionGameplayAbility>>ActivateOnGivenAbilities;

	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UActionGameplayAbility>>ReactiveAbilities;

	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TMap<int32,FActionHitReactionAbilityContainer> HitReactionAbilities;

	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UGameplayEffect>>StartUpGameplayEffects;

	void GrantAbilities(const TArray<TSubclassOf<UActionGameplayAbility>>& InAbilitiesToGive,UActionAbilitySystemComponent* InActionASCToGive,int32 ApplyLevel=1);
	
};
