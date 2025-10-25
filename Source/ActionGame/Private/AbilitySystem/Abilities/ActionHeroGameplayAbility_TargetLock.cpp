// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ActionHeroGameplayAbility_TargetLock.h"

#include "Characters/ActionHeroCharacter.h"
#include "Components/Combat/TargetLockComponent.h"


void UActionHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                            const FGameplayAbilityActorInfo* ActorInfo,
                                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                                            const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (UTargetLockComponent* TargetLockComponent = GetHeroTargetLockComponentFromActorInfo())
	{
		if (!TargetLockComponent->TryLockOnTarget()) CancelTargetLockAbility();

		TargetLockHandle = TargetLockComponent->OnLockStatuesChanged.AddLambda([this](bool bCanContinueLocking)
		{
			if (!bCanContinueLocking) CancelTargetLockAbility();
		});
	}
	else
	{
		CancelTargetLockAbility();
	}
}

void UActionHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                                       const FGameplayAbilityActorInfo* ActorInfo,
                                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                                       bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (UTargetLockComponent* TargetLockComponent = GetHeroTargetLockComponentFromActorInfo())
	{
		TargetLockComponent->OnLockStatuesChanged.Remove(TargetLockHandle);
		TargetLockComponent->CleanUp();
	}
	
}


void UActionHeroGameplayAbility_TargetLock::CancelTargetLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}
