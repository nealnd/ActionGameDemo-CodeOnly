// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionWeaponBase.h"
#include "Library/StructLibrary.h"
#include "ActionHeroWeapon.generated.h"

UCLASS()
class ACTIONGAME_API AActionHeroWeapon : public AActionWeaponBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	UInputMappingContext* WeaponInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	TArray<FActionHeroAbilitySet>DefaultWeaponAbilities;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	TMap<int32,FActionHitReactionAbilityContainer> HitReactionAbilities;

	UFUNCTION(BlueprintCallable)
	void GetAllValidHitReactionAbilities(TArray<TSubclassOf<UActionGameplayAbility>>&Abilities) const;


};
