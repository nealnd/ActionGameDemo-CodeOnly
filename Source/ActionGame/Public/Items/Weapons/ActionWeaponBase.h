// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameFramework/Actor.h"
#include "Library/StructLibrary.h"
#include "ActionWeaponBase.generated.h"

UCLASS()
class ACTIONGAME_API AActionWeaponBase : public AActor
{
	GENERATED_BODY()

public:

	AActionWeaponBase();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="WeaponData")
	FActionWeaponData WeaponData;

	

protected:

	UPROPERTY(visibleAnywhere,BlueprintReadOnly,Category="Weapon")
	UStaticMeshComponent* WeaponMesh;

	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles);

	UFUNCTION(BlueprintPure)
	TArray<FGameplayAbilitySpecHandle> GetGrantedAbilitySpecHandles() const;

private:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilitySpecHandles;
};
