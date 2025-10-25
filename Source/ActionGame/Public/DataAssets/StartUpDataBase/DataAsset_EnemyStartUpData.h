// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAsset_StartUpDataBase.h"
#include "DataAsset_EnemyStartUpData.generated.h"

class UActionEnemyGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UDataAsset_EnemyStartUpData : public UDataAsset_StartUpDataBase
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UActionAbilitySystemComponent* InActionASCToGive,int32 ApplyLevel=1) override;

private:
	UPROPERTY(EditDefaultsOnly,Category="StartUpData")
	TArray<TSubclassOf<UActionEnemyGameplayAbility>>EnemyCombatAbilities;
};
