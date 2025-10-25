// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameplayAbility.h"
#include "ActionEnemyGameplayAbility.generated.h"

class UEnemyCombatComponent;
class AActionEnemyCharacter;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionEnemyGameplayAbility : public UActionGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(blueprintPure,Category="Action|Ability")
	AActionEnemyCharacter* GetEnemyCharacterFromActorInfo() ;

	UFUNCTION(blueprintPure,Category="Action|Ability")
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo() ;

	

private:
	TWeakObjectPtr<AActionEnemyCharacter>CachedActionEnemyCharacter;

};
