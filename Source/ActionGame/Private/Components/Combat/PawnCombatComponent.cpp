// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/PawnCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Components/Combat/AttackCollisionComponent.h"
#include "Components/Combat/HitReactionComponent.h"
#include "Items/Weapons/ActionWeaponBase.h"
#include "Library/ActionFunctionLibrary.h"


void UPawnCombatComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	for (auto& WeaponPair : CharacterCarriedWeaponMap)
	{
		WeaponPair.Value->Destroy();
	}
	for (auto& CollisionComponent : AttackCollisionComponents)
	{
		CollisionComponent->DestroyComponent();
	}
}

void UPawnCombatComponent::RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister,
                                                 AActionWeaponBase* InWeaponToRegister, bool bRegisterAsEquippedWeapon)
{
	checkf(!CharacterCarriedWeaponMap.Contains(InWeaponTagToRegister),
	       TEXT("A named %s has already beed added as carried weapon"), *InWeaponTagToRegister.ToString());
	check(InWeaponToRegister);

	CharacterCarriedWeaponMap.Emplace(InWeaponTagToRegister, InWeaponToRegister);
	if (bRegisterAsEquippedWeapon)
	{
		CurrentEquippedWeaponTag = InWeaponTagToRegister;
	}
}

AActionWeaponBase* UPawnCombatComponent::GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const
{
	if (CharacterCarriedWeaponMap.Contains(InWeaponTagToGet))
	{
		if (AActionWeaponBase* const* FoundWeapon = CharacterCarriedWeaponMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}
	return nullptr;
}

AActionWeaponBase* UPawnCombatComponent::GetCharacterCurrentEquippedWeapon() const
{
	if (!CurrentEquippedWeaponTag.IsValid())
	{
		return nullptr;
	}
	return GetCharacterCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

void UPawnCombatComponent::RegisterSpawnedAttackCollisionComponent(
	UAttackCollisionComponent* InAttackCollisionComponent)
{
	AttackCollisionComponents.AddUnique(InAttackCollisionComponent);
	InAttackCollisionComponent->OnHit.BindUObject(this, &ThisClass::OnMeleeHit);
}

void UPawnCombatComponent::RemoveAttackCollisionComponent(UAttackCollisionComponent* InAttackCollisionComponent)
{
	AttackCollisionComponents.Remove(InAttackCollisionComponent);
}


void UPawnCombatComponent::OnMeleeHit(const FHitResult& HitResult,
                                      const UAttackCollisionComponent* InAttackCollisionComponent)
{
	FGameplayEventData EventData;
	EventData.Instigator = InAttackCollisionComponent->GetOwner();
	EventData.Target = HitResult.GetActor();
	FGameplayAbilityTargetDataHandle TargetDataHandle =
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
	EventData.TargetData = TargetDataHandle;

	HitResultInfo.HitParameters = InAttackCollisionComponent->HitParameters;
	HitResultInfo.HitResult = HitResult;

	FActionHitResultInfo TargetHitResultInfo;
	TargetHitResultInfo.HitResult= HitResult;
	TargetHitResultInfo.HitParameters = InAttackCollisionComponent->HitParameters;
	TargetHitResultInfo.Attacker= InAttackCollisionComponent->GetOwner();
	
	if (APawn* TargetPawn = Cast<APawn>(HitResult.GetActor()))
	{
		if (UActionFunctionLibrary::IsTargetPawnHostile(GetOwningPawn(), TargetPawn))
		{
			bool bIsValidBlockAngle = UActionFunctionLibrary::IsValidBlockAngle(GetOwner(), TargetPawn);
			bool bIsBlocking = UActionFunctionLibrary::NativeDoesActorHaveTag(
				TargetPawn, ActionGameplayTags::Shared_Status_Blocking);

			UPawnCombatComponent* PawnCombatComponent = UActionFunctionLibrary::NativeGetPawnCombatComponentFromActor(TargetPawn);
			check(PawnCombatComponent);
			if (bIsValidBlockAngle && bIsBlocking)
			{
				
				TargetHitResultInfo.bIsBlockSuccessfully = true;
				if (UActionFunctionLibrary::NativeDoesActorHaveTag(
					TargetPawn, ActionGameplayTags::Shared_Status_PerfectBlocking))
				{
					TargetHitResultInfo.bIsPerfectBlockSuccessfully = true;
					PawnCombatComponent->SetHitResultInfo(TargetHitResultInfo);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						TargetPawn, ActionGameplayTags::Shared_Event_PerfectBlockSuccessfully, EventData);
				}
				else
				{
					PawnCombatComponent->SetHitResultInfo(TargetHitResultInfo);
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						TargetPawn, ActionGameplayTags::Shared_Event_BlockSuccessfully, EventData);
				}
				
			}
			/*the target is hit*/
			else
			{
				PawnCombatComponent->SetHitResultInfo(TargetHitResultInfo);
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
					GetOwner(), ActionGameplayTags::Shared_Event_MeleeHit, EventData);
			}
			
		}
	}
	else
	{
		// TODO: Implement other kind of actors
	}
}

void UPawnCombatComponent::SetHitResultInfo(const FActionHitResultInfo InHitResultInfo)
{
	HitResultInfo = InHitResultInfo;
}

void UPawnCombatComponent::GetHitResultInfo(FActionHitResultInfo& OutHitResultInfo) const
{
	OutHitResultInfo = HitResultInfo;
}

void UPawnCombatComponent::SetIsBlockBreak(bool bIsBlockBreak)
{
	HitResultInfo.bIsBlockBreak = bIsBlockBreak;
}
