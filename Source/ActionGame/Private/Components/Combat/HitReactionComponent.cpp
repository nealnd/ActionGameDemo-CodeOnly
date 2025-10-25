// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/HitReactionComponent.h"
#include "AbilitySystem/Abilities/ActionGameplayAbility.h"
#include "Components/Combat/AttackCollisionComponent.h"



void UHitReactionComponent::SetCurrentHitReactions(const TMap<int32, FActionHitReactionAbilityContainer> HitReactionAbilities,
                                                   bool isDefaultHitReactions)
{
	if (isDefaultHitReactions)
		this->DefaultHitReactions = HitReactionAbilities;
	HitReactions = HitReactionAbilities;
}

void UHitReactionComponent::SetDefaultHitReactions()
{
	this->HitReactions = this->DefaultHitReactions;
}

/*TODO: the attack direction need to take into consideration later*/
void UHitReactionComponent::OnCharacterHitReact(const AActor* Attacker,const FActionHitResultInfo& HitResultInfo, TSubclassOf<UGameplayAbility>&AbilityToExecute)
{
	check(!HitReactions.IsEmpty());
	if (HitResultInfo.bIsBlockSuccessfully)
	{
		if (HitResultInfo.bIsBlockBreak)
		{
			AbilityToExecute = HitReactions.Find(HitResultInfo.HitParameters.HitLevel)->BlockBreak.Default;
		}
		else AbilityToExecute = HitReactions.Find(HitResultInfo.HitParameters.HitLevel)->Block.Default;
	}
	else
	{
		const FVector Forward = GetOwner()->GetActorForwardVector();
		FVector ToHit = (Attacker->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
		ToHit.Z = 0.f;
		float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, ToHit)));
		const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
		if (CrossProduct.Z < 0.f)
		{
			Angle *= -1.f;
		}
		if (Angle >= -75.f && Angle <= 75.f)
		{
			AbilityToExecute = HitReactions.Find(HitResultInfo.HitParameters.HitLevel)->Front.Default;
		}
		else if (Angle > 75.f && Angle <= 115.f)
		{
			AbilityToExecute = HitReactions.Find(HitResultInfo.HitParameters.HitLevel)->Left.Default;
		}
		else if (Angle < -75.f && Angle >= -115.f)
		{
			AbilityToExecute = HitReactions.Find(HitResultInfo.HitParameters.HitLevel)->Right.Default;
		}
		else
		{
			AbilityToExecute = HitReactions.Find(HitResultInfo.HitParameters.HitLevel)->Back.Default;
		}
	}

}


