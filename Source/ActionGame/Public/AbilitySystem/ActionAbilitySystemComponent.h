// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Library/StructLibrary.h"
#include "ActionAbilitySystemComponent.generated.h"


class IPawnUIInterface;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UActionAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:


	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category="Action|Ability", meta=(ApplyLevel="1"))
	void GrantHeroWeaponAbilities(const TArray<FActionHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel,
	                              TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
	
	UFUNCTION(BlueprintCallable, Category="Action|Ability")
	void RemoveGrantedWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);

	UFUNCTION(BlueprintCallable, Category="Action|Ability", meta=(ApplyLevel="1"))
	void GrantEnemyWeaponAbilities(const TArray<TSubclassOf<UActionEnemyGameplayAbility>>& InDefaultWeaponAbilities, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintCallable, Category="Action|Ability", meta=(ApplyLevel="1"))
	void GrantWeaponHitReactionAbilities(const TArray<TSubclassOf<UActionGameplayAbility>>& InDefaultWeaponHitReactionAbilities, int32 ApplyLevel,TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
	
	void EnableSkillSwitchWindowStart(const FGameplayEventData* GameplayEventData);
	
	void EnableSkillSwitchWindowEnd(const FGameplayEventData* GameplayEventData);

	void SkillEnd();



	UPROPERTY()
	TArray<const UClass*>SkillToSwitchClasses;

	UPROPERTY()
	TArray<const UInputMappingContext*>InputMappingContexts;
	
};
