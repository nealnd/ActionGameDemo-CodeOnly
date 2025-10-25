// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ActionHeroGameplayAbility.h"


#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Characters/ActionHeroCharacter.h"
#include "Controllers/ActionHeroController.h"


void UActionHeroGameplayAbility::OnEnableSkillSwitch(const FGameplayEventData* GameplayEventData)
{
	UActionAbilitySystemComponent* ASC = Cast<UActionAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	check(ASC);
	ASC->EnableSkillSwitchWindowStart(GameplayEventData);
}

void UActionHeroGameplayAbility::OnDisableSkillSwitch(const FGameplayEventData* GameplayEventData)
{
	UActionAbilitySystemComponent* ASC = Cast<UActionAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	check(ASC);
	ASC->EnableSkillSwitchWindowEnd(GameplayEventData);
}

void UActionHeroGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                 const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilityActivationInfo ActivationInfo,
                                                 const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	GetActionAbilitySystemComponentFromActorInfo()->SkillToSwitchClasses.Empty();
	EnableSkillSwitchDelegateHandle = ActorInfo->AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(
		ActionGameplayTags::Player_Event_EnableSkillSwitch).AddUObject(this, &ThisClass::OnEnableSkillSwitch);
	DisableSkillSwitchDelegateHandle = ActorInfo->AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(
		ActionGameplayTags::Player_Event_DisableSkillSwitch).AddUObject(this, &ThisClass::OnDisableSkillSwitch);
}

void UActionHeroGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                            const FGameplayAbilityActorInfo* ActorInfo,
                                            const FGameplayAbilityActivationInfo ActivationInfo,
                                            bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	
	UActionAbilitySystemComponent* ASC = Cast<UActionAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
	check(ASC);
	
	ASC->SkillEnd();
	ActorInfo->AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(
		ActionGameplayTags::Player_Event_EnableSkillSwitch).Remove(EnableSkillSwitchDelegateHandle);
	ActorInfo->AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(
		ActionGameplayTags::Player_Event_DisableSkillSwitch).Remove(DisableSkillSwitchDelegateHandle);

	
}

AActionHeroCharacter* UActionHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedActionHeroCharacter.IsValid())
	{
		CachedActionHeroCharacter = Cast<AActionHeroCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedActionHeroCharacter.IsValid() ? CachedActionHeroCharacter.Get() : nullptr;
}

AActionHeroController* UActionHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedActionHeroController.IsValid())
	{
		CachedActionHeroController = Cast<AActionHeroController>(CurrentActorInfo->PlayerController);
	}
	return CachedActionHeroController.IsValid() ? CachedActionHeroController.Get() : nullptr;
}

UHeroCombatComponent* UActionHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

UTargetLockComponent* UActionHeroGameplayAbility::GetHeroTargetLockComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetTargetLockComponent();
}
