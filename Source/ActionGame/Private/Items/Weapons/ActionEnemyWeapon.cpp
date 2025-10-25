// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/ActionEnemyWeapon.h"


void AActionEnemyWeapon::GetAllValidHitReactionAbilities(TArray<TSubclassOf<UActionGameplayAbility>>& Abilities) const
{
	check(!HitReactionAbilities.IsEmpty());
	for (const auto& HitReactionAbility : HitReactionAbilities)
	{
		TArray<TSubclassOf<UActionGameplayAbility>> TempAbilities;
		HitReactionAbility.Value.GetAllValidAbilities(TempAbilities);
		Abilities.Append(TempAbilities);
	}
}
