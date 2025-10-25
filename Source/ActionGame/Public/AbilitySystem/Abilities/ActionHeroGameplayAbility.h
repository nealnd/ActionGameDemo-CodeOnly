// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameplayAbility.h"
#include "ActionHeroGameplayAbility.generated.h"

class UTargetLockComponent;
class UInputMappingContext;
class UHeroCombatComponent;
class AActionHeroController;
class AActionHeroCharacter;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionHeroGameplayAbility : public UActionGameplayAbility
{
	GENERATED_BODY()

public:
	void OnEnableSkillSwitch(const FGameplayEventData* GameplayEventData);
	void OnDisableSkillSwitch(const FGameplayEventData* GameplayEventData);
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintPure, Category="Action|Ability")
	AActionHeroCharacter* GetHeroCharacterFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Action|Ability")
	AActionHeroController* GetHeroControllerFromActorInfo();

	UFUNCTION(BlueprintPure, Category="Action|Ability")
	UHeroCombatComponent* GetHeroCombatComponentFromActorInfo();
	
	UFUNCTION(BlueprintPure, Category="Action|Ability")
	UTargetLockComponent* GetHeroTargetLockComponentFromActorInfo();
	

private:
	TWeakObjectPtr<AActionHeroCharacter> CachedActionHeroCharacter;
	TWeakObjectPtr<AActionHeroController> CachedActionHeroController;

	UPROPERTY()
	TArray<const UInputMappingContext*>InputMappingContexts;


	FDelegateHandle EnableSkillSwitchDelegateHandle;

	FDelegateHandle DisableSkillSwitchDelegateHandle;



	
};
