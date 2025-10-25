// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/ActionWeaponBase.h"


// Sets default values
AActionWeaponBase::AActionWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh);
	
}

void AActionWeaponBase::AssignGrantedAbilitySpecHandle(const TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles.Append(InGrantedAbilitySpecHandles);
}

TArray<FGameplayAbilitySpecHandle> AActionWeaponBase::GetGrantedAbilitySpecHandles() const
{
	return GrantedAbilitySpecHandles;
}


