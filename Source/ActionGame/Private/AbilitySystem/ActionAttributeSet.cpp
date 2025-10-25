// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ActionAttributeSet.h"

#include "GameplayEffectExtension.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Characters/ActionBaseCharacter.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/PawnUIComponent.h"
#include "Library/ActionFunctionLibrary.h"

UActionAttributeSet::UActionAttributeSet()
{
	InitCurrentHealth(1.0f);
	InitMaxHealth(1.0f);
	InitCurrentSpirit(1.0f);
	InitMaxSpirit(1.0f);
	InitAttackPower(1.0f);
	InitDefensePower(1.0f);
	
}



void UActionAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	if (!CachedPawnUIInterface.IsValid())
	{
		CachedPawnUIInterface= TWeakInterfacePtr<IPawnUIInterface>(Data.Target.GetAvatarActor());
	}
	
	UPawnUIComponent* PawnUIComponent= CachedPawnUIInterface->GetPawnUIComponent();
	
	Super::PostGameplayEffectExecute(Data);
	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
	
	/*compute the delta between old and new, if it is available*/
	float DeltaValue=0;
	if (Data.EvaluatedData.ModifierOp==EGameplayModOp::Type::Additive)
	{
		DeltaValue=Data.EvaluatedData.Magnitude;
	}

	/*Get the Target Actor, which should be the set's owner*/
	AActor* TargetActor=nullptr;
	AController* TargetController=nullptr;
	AActionBaseCharacter* TargetCharacter=nullptr;
	if (Data.Target.AbilityActorInfo.IsValid()&&Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor=Data.Target.AbilityActorInfo->AvatarActor.Get();
		TargetController=Data.Target.AbilityActorInfo->PlayerController.Get();
		TargetCharacter=Cast<AActionBaseCharacter>(TargetActor);
	}
	
	if (Data.EvaluatedData.Attribute==GetDamageAttribute())
	{
		AActor* SourceActor=nullptr;
		AController* SourceController=nullptr;
		AActionBaseCharacter* SourceCharacter=nullptr;
		if (Source&&Source->AbilityActorInfo.IsValid()&&Source->AbilityActorInfo->AvatarActor.IsValid())
		{
			SourceActor=Source->AbilityActorInfo->AvatarActor.Get();
			SourceController=Source->AbilityActorInfo->PlayerController.Get();
			/*it may not be a player,like AI*/
			if (SourceController==nullptr&&SourceActor!=nullptr)
			{
				if (APawn* Pawn=Cast<APawn>(SourceActor))
				{
					SourceController=Pawn->GetController();
				}
			}
			/*use the controller to find the source pawn*/
			if (SourceController)
			{
				SourceCharacter=Cast<AActionBaseCharacter>(SourceController->GetPawn());
			}
			else
			{
				SourceCharacter=Cast<AActionBaseCharacter>(SourceActor);
			}

			/*set the causer actor,sometimes it is the projectile,trap and so on, not the instigator */
			if (Context.GetEffectCauser())
			{
				SourceActor=Context.GetEffectCauser();
			}
		}

		/*if the FGameplayEffectContext has a hit result*/
		FHitResult HitResult;
		if (Context.GetHitResult())
		{
			HitResult=*Context.GetHitResult();
		}

		/*store a local copy of the amount of damage and clear the damage attribute*/
		const float LocalDamageDone=GetDamage();
		SetDamage(0.f);

		if (LocalDamageDone>0)
		{
			/*Apply the health damage and then clamp it*/
			const float OldHealth=GetCurrentHealth();
			SetCurrentHealth(FMath::Clamp(OldHealth-LocalDamageDone,0.0f,GetMaxHealth()));
			//PawnUIComponent->OnHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
		}
	}
	else if (Data.EvaluatedData.Attribute==GetCurrentHealthAttribute())
	{
		/*handle other health changes like from healing*/
		SetCurrentHealth(FMath::Clamp(GetCurrentHealth(),0.0f,GetMaxHealth()));
		//PawnUIComponent->OnHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());

	}
	else if (Data.EvaluatedData.Attribute==GetCurrentSpiritAttribute())
	{
		
		SetCurrentSpirit(FMath::Clamp(GetCurrentSpirit(),0.f,GetMaxSpirit()));
		// if (UHeroUIComponent* HeroUIComponent= CachedPawnUIInterface->GetHeroUIComponent())
		// {
		// 	HeroUIComponent->OnSpiritChanged.Broadcast(GetCurrentSpirit()/GetMaxSpirit());
		// }
	}
	else if (Data.EvaluatedData.Attribute==GetMaxHealthAttribute())
	{
		if (GetCurrentHealth()>GetMaxHealth())
		{
			SetCurrentHealth(FMath::Clamp(GetCurrentHealth(),0.0f,GetMaxHealth()));
		}
		
		// PawnUIComponent->OnHealthChanged.Broadcast(GetCurrentHealth()/GetMaxHealth());
	}
	else if (Data.EvaluatedData.Attribute==GetMaxSpiritAttribute())
	{
		SetCurrentSpirit(FMath::Clamp(GetCurrentSpirit(),0.f,GetMaxSpirit()));
		// if (UHeroUIComponent* HeroUIComponent= CachedPawnUIInterface->GetHeroUIComponent())
		// {
		// 	HeroUIComponent->OnSpiritChanged.Broadcast(GetCurrentSpirit()/GetMaxSpirit());
		// }
	}
	if (GetCurrentHealth()==0.f)
	{
		UActionFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(),ActionGameplayTags::Shared_Status_Death);
		
	}
}

