// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Library/EnumLibrary.h"

#include "ActionGameplayAbility.generated.h"

class UActionAbilitySystemComponent;
class UPawnCombatComponent;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:

	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	

	UPROPERTY(EditDefaultsOnly,Category="Action|Ability")
	EActionAbilityActivationPolicy AbilityActivationPolicy = EActionAbilityActivationPolicy::OnTriggered;

	UFUNCTION(BlueprintPure,Category="Action|Ability")
	UPawnCombatComponent* GetPawnCombatComponentFromActorInfo()const;
	
	UFUNCTION(BlueprintPure,Category="Action|Ability")
	UActionAbilitySystemComponent* GetActionAbilitySystemComponentFromActorInfo()const;
	
	UFUNCTION(BlueprintPure,Category="Action|Ability")
	FGameplayEffectSpecHandle MakeHitDamageEffectSpecHandle(TSubclassOf<UGameplayEffect>EffectClass) const;

	UFUNCTION(BlueprintPure,Category="Action|Ability")
	FGameplayEffectSpecHandle MakeBlockSpiritEffectSpecHandleFromHitInfo(TSubclassOf<UGameplayEffect>EffectClass) const;

	UFUNCTION(BlueprintPure,Category="Action|Ability")
	FGameplayEffectSpecHandle MakePerfectBlockSpiritEffectSpecHandleFromHitInfo(TSubclassOf<UGameplayEffect>EffectClass) const;

	


	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle) const;

	UFUNCTION(BlueprintCallable,Category="Action|Ability",meta=(DisplayName="Apply Effect Spec Handle To Target",ExpandEnumAsExecs="OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,const FGameplayEffectSpecHandle& SpecHandle,EActionSuccessType& OutSuccessType) const;
	
	
};
