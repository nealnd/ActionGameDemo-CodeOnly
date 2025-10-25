// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionWeaponBase.h"
#include "ActionEnemyWeapon.generated.h"

UCLASS()
class ACTIONGAME_API AActionEnemyWeapon : public AActionWeaponBase
{
	GENERATED_BODY()


public:
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	TArray<TSubclassOf<UActionEnemyGameplayAbility>>DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	TMap<int32,FActionHitReactionAbilityContainer> HitReactionAbilities;

	UFUNCTION(BlueprintCallable)
	void GetAllValidHitReactionAbilities(TArray<TSubclassOf<UActionGameplayAbility>>&Abilities) const;
};
