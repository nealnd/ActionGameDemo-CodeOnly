// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpDataBase/DataAsset_StartUpDataBase.h"

#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ActionGameplayAbility.h"

void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InActionASCToGive,
                                                              int32 ApplyLevel)
{
	check(InActionASCToGive);
	GrantAbilities(ActivateOnGivenAbilities, InActionASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InActionASCToGive, ApplyLevel);
	for (const auto& HitReactionAbility : HitReactionAbilities)
	{
		TArray<TSubclassOf<UActionGameplayAbility>>Abilities;
		HitReactionAbility.Value.GetAllValidAbilities(Abilities);
		GrantAbilities(Abilities, InActionASCToGive, ApplyLevel);
	}
	if (!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if (!EffectClass) continue;

			UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InActionASCToGive->ApplyGameplayEffectToSelf(EffectCDO, ApplyLevel, InActionASCToGive->MakeEffectContext());
		}
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UActionGameplayAbility>>& InAbilitiesToGive,
                                                UActionAbilitySystemComponent* InActionASCToGive, int32 ApplyLevel)
{
	if (InAbilitiesToGive.IsEmpty()) return;
	for (const TSubclassOf<UActionGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if (!Ability) continue;
		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InActionASCToGive->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		InActionASCToGive->GiveAbility(AbilitySpec);
	}
}
