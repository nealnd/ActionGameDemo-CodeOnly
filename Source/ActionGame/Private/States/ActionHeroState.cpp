// Fill out your copyright notice in the Description page of Project Settings.


#include "States/ActionHeroState.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/ActionAttributeSet.h"


AActionHeroState::AActionHeroState()
{
	AbilitySystemComponent=CreateDefaultSubobject<UActionAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet=CreateDefaultSubobject<UActionAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AActionHeroState::GetAbilitySystemComponent() const
{
	return GetActionAbilitySystemComponent();
}
