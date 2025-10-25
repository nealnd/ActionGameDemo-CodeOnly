// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PawnCombatComponent.h"
#include "EnemyCombatComponent.generated.h"



class AActionEnemyWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UEnemyCombatComponent : public UPawnCombatComponent
{
	GENERATED_BODY()
	
public:

	
	UFUNCTION(BlueprintCallable,Category="Action|Combat")
	AActionEnemyWeapon* GetEnemyCarriedWeaponByTag(FGameplayTag InWeaponTag) const;

	UFUNCTION(BlueprintCallable,Category="Action|Combat")
	AActionEnemyWeapon* GetEnemyCurrentEquippedWeapon() const;


protected:
	
	



	
};
