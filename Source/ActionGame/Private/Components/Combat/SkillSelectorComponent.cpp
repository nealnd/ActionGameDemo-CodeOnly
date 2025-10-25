// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/SkillSelectorComponent.h"

#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "ActionGame/DebugHelper.h"
#include "Controllers/ActionAIController.h"


void USkillSelectorComponent::InitializeComponent()
{
	Super::InitializeComponent();
	OnExecutingAbility.Unbind();
	for (FActionAIWeightedSkillConfig& WeightedSkillConfig : DefaultWeightedSkillConfigContainer.SkillConfigs)
	{
		for (auto& Skill : WeightedSkillConfig.SkillData)
		{
			Skill.Value.CurrentWeight = Skill.Value.DefaultWeight;
		}
	}
	OnExecutingAbility.BindLambda([this](UClass* AbilityClass)
	{
		for (FActionAIWeightedSkillConfig& WeightedSkillConfig : DefaultWeightedSkillConfigContainer.SkillConfigs)
		{
			if (WeightedSkillConfig.IsValid() && !WeightedSkillConfig.SkillData.IsEmpty())
			{
				if (WeightedSkillConfig.SkillData.Contains(AbilityClass))
				{
					if (WeightedSkillConfig.SkillData[AbilityClass].bIsResetOnCooldown)
					{
						float CooldownTime = WeightedSkillConfig.SkillData[AbilityClass].CooldownTime;
						RollbackWeightAfterCooldown(AbilityClass, CooldownTime);
						WeightedSkillConfig.SkillData[AbilityClass].CurrentWeight = 0.f;
						//Debug::Print("The weight of " + AbilityClass->GetName() + " is reset to 0");
					}
				}
			}
		}
	});
}

void USkillSelectorComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}


TSubclassOf<UGameplayAbility> USkillSelectorComponent::RandomAppropriateAbility(float InDistance, float InAngle)
{
	TMap<TSubclassOf<UGameplayAbility>, FActionAIWeightedSkillParameter> AppropriateAbilities;
	GetAppropriateAbilities(InDistance, InAngle, AppropriateAbilities);
	float Sum = 0.f;
	if (AppropriateAbilities.IsEmpty()) return nullptr;
	for (TPair<TSubclassOf<UGameplayAbility>, FActionAIWeightedSkillParameter>& Ability : AppropriateAbilities)
	{
		Sum += Ability.Value.DefaultWeight;
	}

	float Value = FMath::RandRange(0.f, Sum);
	float CurrentValue = 0;
	for (auto& Ability : AppropriateAbilities)
	{
		CurrentValue += Ability.Value.DefaultWeight;
		if (Value <= CurrentValue)
		{
			return Ability.Key;
		}
	}
	return nullptr;
}

void USkillSelectorComponent::GetAppropriateAbilities(float InDistance, float InAngle,
                                                      TMap<TSubclassOf<UGameplayAbility>,
                                                           FActionAIWeightedSkillParameter>& InAppropriateAbilities)
{
	if (!ActionAbilitySystemComponent.IsValid())
	{
		if (AActionAIController* ActionAIController = Cast<AActionAIController>(GetOwner()))
		{
			ActionAbilitySystemComponent = ActionAIController->GetActionASCFromAIController();
		}
		else return;
	}
	for (const FActionAIWeightedSkillConfig& WeightedSkillConfig : DefaultWeightedSkillConfigContainer.SkillConfigs)
	{
		if (WeightedSkillConfig.IsValid())
		{
			if (WeightedSkillConfig.MinDistance < InDistance && WeightedSkillConfig.MaxDistance >= InDistance)
			{
				if ((WeightedSkillConfig.MinAngle < WeightedSkillConfig.MaxAngle && InAngle >= WeightedSkillConfig.
						MinAngle && InAngle <= WeightedSkillConfig.MaxAngle)
					|| (WeightedSkillConfig.MinAngle > WeightedSkillConfig.MaxAngle && (InAngle > WeightedSkillConfig.
						MinAngle || InAngle < WeightedSkillConfig.MaxAngle)))
				{
					for (const TPair<TSubclassOf<UGameplayAbility>, FActionAIWeightedSkillParameter>& Skill :
					     WeightedSkillConfig.SkillData)
					{
						if (Skill.Value.CurrentWeight > 0.f)
						{
							if (ActionAbilitySystemComponent->FindAbilitySpecFromClass(Skill.Key))
							{
								InAppropriateAbilities.Add(Skill);
							}
						}
					}
				}
			}
		}
	}
}

void USkillSelectorComponent::RollbackWeightAfterCooldown(UClass* AbilityClass, float CooldownTime)
{
	FTimerHandle TimerHandle;
	TWeakObjectPtr<USkillSelectorComponent> WeakThis(this);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda(
		                                       [WeakThis,AbilityClass,CooldownTime]()
		                                       {
			                                       if (!WeakThis.IsValid()) return;
			                                       USkillSelectorComponent* Component = WeakThis.Get();
			                                       if (Component->DefaultWeightedSkillConfigContainer.SkillConfigs.
			                                                      IsEmpty()) return;

			                                       for (FActionAIWeightedSkillConfig& WeightedSkillConfig :
			                                            Component->DefaultWeightedSkillConfigContainer.SkillConfigs)
			                                       {
				                                       if (WeightedSkillConfig.IsValid() && !WeightedSkillConfig.
					                                       SkillData.IsEmpty())
				                                       {
					                                       if (WeightedSkillConfig.SkillData.Contains(AbilityClass))
					                                       {
						                                       WeightedSkillConfig.SkillData[AbilityClass].CurrentWeight
							                                       =
							                                       WeightedSkillConfig.SkillData[AbilityClass].
							                                       DefaultWeight;
					                                       }
				                                       }
			                                       }
		                                       }), CooldownTime, false);
}
