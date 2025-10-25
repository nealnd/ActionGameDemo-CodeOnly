// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/EnemyCombatComponent.h"
#include "Items/Weapons/ActionEnemyWeapon.h"
#include "Library/ActionFunctionLibrary.h"




AActionEnemyWeapon* UEnemyCombatComponent::GetEnemyCarriedWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<AActionEnemyWeapon>(GetCharacterCarriedWeaponByTag(InWeaponTag));
}

AActionEnemyWeapon* UEnemyCombatComponent::GetEnemyCurrentEquippedWeapon() const
{
	return Cast<AActionEnemyWeapon>(GetCharacterCurrentEquippedWeapon());
}




