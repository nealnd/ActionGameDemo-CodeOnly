// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ActionGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayAbilityBlueprint.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Components/Combat/AttackCollisionComponent.h"
#include "Components/Combat/HitReactionComponent.h"
#include "Components/Combat/PawnCombatComponent.h"

void UActionGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	if (AbilityActivationPolicy == EActionAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo && !Spec.IsActive())
		{
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UActionGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo,
                                        const FGameplayAbilityActivationInfo ActivationInfo,
                                        bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
	if (AbilityActivationPolicy == EActionAbilityActivationPolicy::OnGiven)
	{
		if (ActorInfo)
		{
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}


UPawnCombatComponent* UActionGameplayAbility::GetPawnCombatComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UPawnCombatComponent>();
}

UActionAbilitySystemComponent* UActionGameplayAbility::GetActionAbilitySystemComponentFromActorInfo() const
{
	return Cast<UActionAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}




FGameplayEffectSpecHandle UActionGameplayAbility::MakeHitDamageEffectSpecHandle(
	TSubclassOf<UGameplayEffect> EffectClass) const
{
	check(EffectClass);

	float SkillDamagePercentage = 0.f;
	FHitResult HitResult;

	if (UPawnCombatComponent* PawnCombatComponent = GetPawnCombatComponentFromActorInfo())
	{
		FActionHitResultInfo HitResultInfo;
		PawnCombatComponent->GetHitResultInfo(HitResultInfo);
		SkillDamagePercentage = HitResultInfo.HitParameters.SKillDamagePercentage;
		HitResult=HitResultInfo.HitResult;
	}

	FGameplayEffectContextHandle EffectContextHandle = GetActionAbilitySystemComponentFromActorInfo()->
		MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(GetOwningActorFromActorInfo());
	EffectContextHandle.AddHitResult(HitResult,false);
	//check(Payload.TargetData.Get(0));
	// EffectContextHandle.AddHitResult(*Payload.TargetData.Get(0)->GetHitResult(), false);
	EffectContextHandle.AddInstigator(GetOwningActorFromActorInfo(), GetOwningActorFromActorInfo());
	FGameplayEffectSpecHandle EffectSpecHandle = GetActionAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass, GetAbilityLevel(), EffectContextHandle);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(ActionGameplayTags::Shared_SetByCaller_SkillDamagePercentage,
	                                               SkillDamagePercentage);

	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UActionGameplayAbility::MakeBlockSpiritEffectSpecHandleFromHitInfo(
	TSubclassOf<UGameplayEffect> EffectClass) const
{
	check(EffectClass);

	float BlockSpiritCost = 0.f;
	FHitResult HitResult;
	if (UPawnCombatComponent* PawnCombatComponent = GetPawnCombatComponentFromActorInfo())
	{
		FActionHitResultInfo HitResultInfo;
		PawnCombatComponent->GetHitResultInfo(HitResultInfo);
		BlockSpiritCost = HitResultInfo.HitParameters.BlockSpiritCost;
		HitResult=HitResultInfo.HitResult;
	}
	FGameplayEffectContextHandle EffectContextHandle = GetActionAbilitySystemComponentFromActorInfo()->
		MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(GetOwningActorFromActorInfo());
	EffectContextHandle.AddHitResult(HitResult,false);
	//check(Payload.TargetData.Get(0));
	//EffectContextHandle.AddHitResult(*Payload.TargetData.Get(0)->GetHitResult(), false);
	EffectContextHandle.AddInstigator(GetOwningActorFromActorInfo(), GetOwningActorFromActorInfo());
	FGameplayEffectSpecHandle EffectSpecHandle = GetActionAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass, GetAbilityLevel(), EffectContextHandle);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(ActionGameplayTags::Shared_SetByCaller_BlockSpiritCost,
												  -BlockSpiritCost);
	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UActionGameplayAbility::MakePerfectBlockSpiritEffectSpecHandleFromHitInfo(
	TSubclassOf<UGameplayEffect> EffectClass) const
{
	check(EffectClass);

	float PerfectBlockSpiritGain = 0.f;
	FHitResult HitResult;
	if (UPawnCombatComponent* PawnCombatComponent = GetPawnCombatComponentFromActorInfo())
	{
		FActionHitResultInfo HitResultInfo;
		PawnCombatComponent->GetHitResultInfo(HitResultInfo);
		PerfectBlockSpiritGain = HitResultInfo.HitParameters.PerfectBlockSpiritGain;
		HitResult=HitResultInfo.HitResult;
	}
	FGameplayEffectContextHandle EffectContextHandle = GetActionAbilitySystemComponentFromActorInfo()->
		MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(GetOwningActorFromActorInfo());
	EffectContextHandle.AddHitResult(HitResult,false);
	//check(Payload.TargetData.Get(0));
	//EffectContextHandle.AddHitResult(*Payload.TargetData.Get(0)->GetHitResult(), false);
	EffectContextHandle.AddInstigator(GetOwningActorFromActorInfo(), GetOwningActorFromActorInfo());
	FGameplayEffectSpecHandle EffectSpecHandle = GetActionAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass, GetAbilityLevel(), EffectContextHandle);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(ActionGameplayTags::Shared_SetByCaller_PerfectBlockSpiritGain,
												  PerfectBlockSpiritGain);
	return EffectSpecHandle;
}


FActiveGameplayEffectHandle UActionGameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* TargetActor,
                                                                                        const FGameplayEffectSpecHandle& SpecHandle) const
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	check(TargetASC && SpecHandle.IsValid());
	return GetActionAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*SpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UActionGameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& SpecHandle, EActionSuccessType& OutSuccessType) const
{
	FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleToTarget(
		TargetActor, SpecHandle);
	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied()
		                 ? EActionSuccessType::Successful
		                 : EActionSuccessType::Failed;
	return ActiveGameplayEffectHandle;
}
