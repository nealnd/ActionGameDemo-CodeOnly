// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/PawnExtensionComponentBase.h"
#include "Library/StructLibrary.h"

#include "PawnCombatComponent.generated.h"

struct FGameplayEventData;
DECLARE_DELEGATE_OneParam(FOnExecutingAbility, UClass*);

class UActionAbilitySystemComponent;
class UAttackCollisionComponent;
class AActionWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UPawnCombatComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
	UFUNCTION(BlueprintCallable, Category="Action|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, AActionWeaponBase* InWeaponToRegister,
	                           bool bRegisterAsEquippedWeapon = false);

	UFUNCTION(BlueprintCallable, Category="Action|Combat")
	AActionWeaponBase* GetCharacterCarriedWeaponByTag(FGameplayTag InWeaponTagToGet) const;

	UPROPERTY(BlueprintReadWrite, Category="Action|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category="Action|Combat")
	AActionWeaponBase* GetCharacterCurrentEquippedWeapon() const;

	UFUNCTION(BlueprintCallable, Category="Action|Combat")
	void RegisterSpawnedAttackCollisionComponent(UAttackCollisionComponent* InAttackCollisionComponent);

	UFUNCTION(BlueprintCallable, Category="Action|Combat")
	void RemoveAttackCollisionComponent(UAttackCollisionComponent* InAttackCollisionComponent);

	virtual void OnMeleeHit(const FHitResult& HitResult,const UAttackCollisionComponent* InAttackCollisionComponent);

	FOnExecutingAbility OnExecutingAbility;
	
	void SetHitResultInfo(const FActionHitResultInfo InHitResultInfo);

	UFUNCTION(BlueprintPure,Category="Action|Combat")
	void GetHitResultInfo(FActionHitResultInfo& OutHitResultInfo) const;

	UFUNCTION(BlueprintCallable,Category="Action|Combat")
	void SetIsBlockBreak(bool bIsBlockBreak);
	
private:
	UPROPERTY()
	TMap<FGameplayTag, AActionWeaponBase*> CharacterCarriedWeaponMap;

	UPROPERTY()
	TArray<UAttackCollisionComponent*> AttackCollisionComponents;
	
	UPROPERTY()
	FActionHitResultInfo HitResultInfo;

	

};
