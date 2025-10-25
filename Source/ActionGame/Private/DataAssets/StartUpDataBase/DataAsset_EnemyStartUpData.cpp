// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpDataBase/DataAsset_EnemyStartUpData.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ActionEnemyGameplayAbility.h"

void UDataAsset_EnemyStartUpData::GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InActionASCToGive,
	int32 ApplyLevel)
{
	Super::GiveToAbilitySystemComponent(InActionASCToGive, ApplyLevel);
	if (!EnemyCombatAbilities.IsEmpty())
	{
		for (const TSubclassOf<UActionEnemyGameplayAbility>& AbilityClass : EnemyCombatAbilities)
		{
			if (!AbilityClass) continue;
			FGameplayAbilitySpec AbilitySpec(AbilityClass);
			AbilitySpec.SourceObject = InActionASCToGive->GetAvatarActor();
			AbilitySpec.Level = ApplyLevel;
			
			InActionASCToGive->GiveAbility(AbilitySpec);
		}
	}
}
