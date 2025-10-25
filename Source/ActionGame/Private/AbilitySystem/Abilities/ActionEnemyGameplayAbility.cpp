// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ActionEnemyGameplayAbility.h"

#include "Characters/ActionEnemyCharacter.h"

AActionEnemyCharacter* UActionEnemyGameplayAbility::GetEnemyCharacterFromActorInfo() 
{
	if (!CachedActionEnemyCharacter.IsValid())
	{
		CachedActionEnemyCharacter = Cast<AActionEnemyCharacter>(CurrentActorInfo->AvatarActor);
	}
	return CachedActionEnemyCharacter.IsValid()?CachedActionEnemyCharacter.Get():nullptr;
		
}

UEnemyCombatComponent* UActionEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo() 
{
	return  GetEnemyCharacterFromActorInfo()->GetEnemyCombatComponentFromActorInfo();
}
