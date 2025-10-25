// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/ActionAttributeSet.h"
#include "ActionGame/ActionGameplayTags.h"

struct FActionDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FActionDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UActionAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UActionAttributeSet, DefensePower, Target, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UActionAttributeSet, Damage, Target, false);
	}
};

static const FActionDamageCapture& GetActionDamageCapture()
{
	static FActionDamageCapture ActionDamageCapture;
	return ActionDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	/*slow method*/
	/*
	 FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UActionAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UActionAttributeSet, AttackPower));

	FGameplayEffectAttributeCaptureDefinition AttackPowerCapture(AttackPowerProperty, EGameplayEffectAttributeCaptureSource::Source, true);
	
	RelevantAttributesToCapture.Add(AttackPowerCapture);
	*/

	RelevantAttributesToCapture.Add(GetActionDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetActionDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetActionDamageCapture().DamageDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                                     FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();


	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetActionDamageCapture().AttackPowerDef,
	                                                           EvaluateParameters, SourceAttackPower);

	float SkillDamagePercentage = 0.f;
	for (const auto& TagMagnitude:EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(ActionGameplayTags::Shared_SetByCaller_SkillDamagePercentage))
		{
			SkillDamagePercentage=TagMagnitude.Value;
		}
		
	}
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetActionDamageCapture().DefensePowerDef,
															   EvaluateParameters, TargetDefensePower);

	const float FinalDamageDone = SourceAttackPower * SkillDamagePercentage *(1-TargetDefensePower/(TargetDefensePower+100.f));

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(GetActionDamageCapture().DamageProperty, EGameplayModOp::Override, FinalDamageDone));
	
}
