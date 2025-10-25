// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ActionAbilitySystemComponent.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "AbilitySystem/Abilities/ActionEnemyGameplayAbility.h"
#include "AbilitySystem/Abilities/ActionGameplayAbility.h"
#include "ActionGame/ActionGameplayTags.h"



void UActionAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		if (InInputTag.MatchesTag(ActionGameplayTags::InputTag_Toggleable))
		{
			if (AbilitySpec.IsActive()) CancelAbilityHandle(AbilitySpec.Handle);
			else TryActivateAbility(AbilitySpec.Handle);
		}
		else
		{
			if (HasMatchingGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch))
			{
				if (SkillToSwitchClasses.Contains(AbilitySpec.Ability.GetClass()))
				{
					/*like dodge transit to dodge,need to cancel current dodge*/
					if (AbilitySpec.IsActive() && AbilitySpec.Ability->GetInstancingPolicy() ==
						EGameplayAbilityInstancingPolicy::InstancedPerActor)
					{
						if (AbilitySpec.Ability->CanActivateAbility(AbilitySpec.Handle, AbilityActorInfo.Get()))
						{
							CancelAbilityHandle(AbilitySpec.Handle);
						}
					}
					TryActivateAbility(AbilitySpec.Handle);
				}
				else return;
			}

			TryActivateAbility(AbilitySpec.Handle);
		}

	}
}

void UActionAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()||!InInputTag.MatchesTag(ActionGameplayTags::InputTag_MustBeHeld)) return;
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)&&AbilitySpec.IsActive())
		{
			FGameplayEventData GameplayEventData;
			HandleGameplayEvent(ActionGameplayTags::Player_Event_InputRelease,&GameplayEventData);
		}
	}
	
}

void UActionAbilitySystemComponent::GrantHeroWeaponAbilities(
	const TArray<FActionHeroAbilitySet>& InDefaultWeaponAbilities, int32 ApplyLevel,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	for (const FActionHeroAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UActionAbilitySystemComponent::RemoveGrantedWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty())return;

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : InSpecHandlesToRemove)
	{
		if (AbilitySpecHandle.IsValid()) ClearAbility(AbilitySpecHandle);
	}
	InSpecHandlesToRemove.Empty();
}

void UActionAbilitySystemComponent::GrantEnemyWeaponAbilities(
	const TArray<TSubclassOf<UActionEnemyGameplayAbility>>& InDefaultWeaponAbilities, int32 ApplyLevel,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)

{
	if (InDefaultWeaponAbilities.IsEmpty()) return;
	for (const TSubclassOf<UActionEnemyGameplayAbility>& AbilityClass : InDefaultWeaponAbilities)
	{
		if (!AbilityClass) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityClass);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UActionAbilitySystemComponent::GrantWeaponHitReactionAbilities(
	const TArray<TSubclassOf<UActionGameplayAbility>>& InDefaultWeaponHitReactionAbilities, int32 ApplyLevel,
	TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	if (InDefaultWeaponHitReactionAbilities.IsEmpty()) return;
	for (const TSubclassOf<UActionGameplayAbility>& AbilityClass : InDefaultWeaponHitReactionAbilities)
	{
		if (!AbilityClass) continue;

		FGameplayAbilitySpec AbilitySpec(AbilityClass);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		OutGrantedAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UActionAbilitySystemComponent::EnableSkillSwitchWindowStart(const FGameplayEventData* GameplayEventData)
{
	if (!HasMatchingGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch))
	{
		AddLooseGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch);
	}
	const UInputMappingContext* InputMappingContext = Cast<UInputMappingContext>(GameplayEventData->OptionalObject);
	check(InputMappingContext);
	InputMappingContexts.AddUnique(InputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(AbilityActorInfo->PlayerController->GetLocalPlayer());

	if (!EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 2);
	}
	const UClass* RawClass = Cast<UClass>(GameplayEventData->OptionalObject2);
	// TSubclassOf<UGameplayAbility> AbilityClass = const_cast<UClass*>(RawClass);
	SkillToSwitchClasses.AddUnique(RawClass);
}

void UActionAbilitySystemComponent::EnableSkillSwitchWindowEnd(const FGameplayEventData* GameplayEventData)
{
	// if (!HasMatchingGameplayTag(ActionGameplayTags::Player_Status_ExecutingSkill))
	// {
	// 	AddLooseGameplayTag(ActionGameplayTags::Player_Status_ExecutingSkill);
	// }
	if (HasMatchingGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch))
	{
		RemoveLooseGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch);
	}
	const UInputMappingContext* InputMappingContext = Cast<UInputMappingContext>(GameplayEventData->OptionalObject);
	check(InputMappingContext);
	InputMappingContexts.Remove(InputMappingContext);
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(AbilityActorInfo->PlayerController->GetLocalPlayer());
	if (EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
	{
		EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
	}
	const UClass* RawClass = Cast<UClass>(GameplayEventData->OptionalObject2);
	// TSubclassOf<UGameplayAbility> AbilityClass = const_cast<UClass*>(RawClass);
	SkillToSwitchClasses.Remove(RawClass);
}

void UActionAbilitySystemComponent::SkillEnd()
{
	if (HasMatchingGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch))
	{
		RemoveLooseGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch);
	}
	if (!InputMappingContexts.IsEmpty())
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(AbilityActorInfo->PlayerController->GetLocalPlayer());
		for (const UInputMappingContext* InputMappingContext : InputMappingContexts)
		{
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

