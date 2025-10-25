// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "ActionAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)


class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UActionAttributeSet();
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category="Spirit")
	FGameplayAttributeData CurrentSpirit;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, CurrentSpirit)

	UPROPERTY(BlueprintReadOnly, Category="Spirit")
	FGameplayAttributeData MaxSpirit;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, MaxSpirit)

	UPROPERTY(BlueprintReadOnly, Category="Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category="Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, DefensePower)

	UPROPERTY(BlueprintReadOnly,Category="Damage")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UActionAttributeSet, Damage);



private:
	TWeakInterfacePtr<IPawnUIInterface>CachedPawnUIInterface;
};
