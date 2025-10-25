// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitReactionInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Interfaces/PawnUIInterface.h"
#include "Interfaces/TargetLockInterface.h"
#include "Library/EnumLibrary.h"
#include "Library/StructLibrary.h"
#include "ActionBaseCharacter.generated.h"

class UHitReactionComponent;
class UMotionWarpingComponent;
class UAttackCollisionComponent;
class UDataAsset_StartUpDataBase;
class UActionAttributeSet;
class UActionAbilitySystemComponent;
class UPhysicalAnimationComponent;

UCLASS()
class ACTIONGAME_API AActionBaseCharacter : public ACharacter, public IAbilitySystemInterface,
                                            public IPawnCombatInterface, public IPawnUIInterface,
                                            public IHitReactionInterface,public ITargetLockInterface
{
	GENERATED_BODY()

public:
	AActionBaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const;

	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;

	virtual UPawnUIComponent* GetPawnUIComponent() const override;

	virtual UHitReactionComponent* GetHitReactionComponent() const override;

	virtual UTargetLockComponent* GetTargetLockComponent() const override;

	UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

protected:
	void OnCharacterHitReact(const FGameplayEventData* GameplayEventData) const;
	virtual void PossessedBy(AController* NewController) override;


	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CharacterData")
	TSoftObjectPtr<UDataAsset_StartUpDataBase> CharacterStartUpData;


	void OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;


#pragma region Locomotion

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Locomotion")
	EGait CurrentGait = EGait::Run;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Locomotion")
	TMap<EGait, FGaitSettings> CurrentGaitSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	TMap<EGait, FGaitSettings> DefaultGaitSettings;

	/*when equip sword, as the animation has different stop animation, we need to change the gait settings*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Locomotion")
	TMap<EGait, FGaitSettings> SwordGaitSettings;

public:
	UFUNCTION(BlueprintCallable, Category="Locomotion")
	void UpdateGait(EGait DesiredGait);

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	void ChangeGaitSettings(const TMap<EGait, FGaitSettings>& InGaitSettings);

#pragma endregion

#pragma region AbilitySystem

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UActionAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UActionAttributeSet> AttributeSet;

public:
	FORCEINLINE UActionAbilitySystemComponent* GetActionAbilitySystemComponent() const
	{
		return AbilitySystemComponent;
	}

	FORCEINLINE UActionAttributeSet* GetActionAttributeSet() const { return AttributeSet; }


#pragma endregion

#pragma region MotionWarping

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MotionWarping")
	UMotionWarpingComponent* MotionWarpingComponent;


#pragma endregion

#pragma region HitReactions

protected:
	/**
	 * 
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HitReaction")
	UHitReactionComponent* HitReactionComponent;

public:
	UFUNCTION(BlueprintCallable, Category="HitReaction")
	UHitReactionComponent* GetHitReactionComponent();

#pragma endregion

#pragma region TargetLock

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="HitReaction")
	UTargetLockComponent* TargetLockComponent;

public:
	UFUNCTION(BlueprintCallable, Category="HitReaction")
	UTargetLockComponent* GetTargetLockComponent();

#pragma endregion
};
