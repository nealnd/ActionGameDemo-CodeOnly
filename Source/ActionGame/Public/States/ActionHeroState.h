// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ActionHeroState.generated.h"

class UActionAttributeSet;
class UActionAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionHeroState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AActionHeroState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

#pragma region AbilitySystem
protected:
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UActionAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="AbilitySystem")
	TObjectPtr<UActionAttributeSet> AttributeSet;

public:

	FORCEINLINE UActionAbilitySystemComponent* GetActionAbilitySystemComponent() const {return AbilitySystemComponent;}
	FORCEINLINE UActionAttributeSet* GetActionAttributeSet() const {return AttributeSet;}
	

#pragma endregion

};
