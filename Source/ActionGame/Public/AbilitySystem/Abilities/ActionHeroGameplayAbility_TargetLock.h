// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionHeroGameplayAbility.h"
#include "ActionHeroGameplayAbility_TargetLock.generated.h"

class UActionWidgetBase;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionHeroGameplayAbility_TargetLock : public UActionHeroGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
							bool bWasCancelled) override;

	

private:

	void CancelTargetLockAbility();

	FDelegateHandle TargetLockHandle;



};

