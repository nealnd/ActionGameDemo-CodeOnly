// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/ActionBaseCharacter.h"
#include "ActionHeroCharacter.generated.h"

class UHeroUIComponent;
class UHeroCombatComponent;
struct FInputActionValue;
class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveInputTriggered, const FVector&);

UCLASS()
class ACTIONGAME_API AActionHeroCharacter : public AActionBaseCharacter
{
	GENERATED_BODY()

public:
	AActionHeroCharacter();

	virtual UPawnCombatComponent* GetPawnCombatComponent()const override;

	virtual UPawnUIComponent* GetPawnUIComponent()const override;
	virtual UHeroUIComponent* GetHeroUIComponent()const override;

	USpringArmComponent* GetSprintArmComponent() const { return CameraBoom; }

	FOnMoveInputTriggered OnMoveInputTriggered;
	
protected:

	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void OnCurrentSpiritChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;
	void OnMaxSpiritChanged(const FOnAttributeChangeData& OnAttributeChangeData) const;

	

private:
	
#pragma region Components

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera",meta=(AllowPrivateAccess=true))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Camera",meta=(AllowPrivateAccess=true))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat",meta=(AllowPrivateAccess=true))
	UHeroCombatComponent* HeroCombatComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat",meta=(AllowPrivateAccess=true))
	UHeroUIComponent* HeroUIComponent;
	
#pragma endregion



#pragma region Inputs

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="CharacterData",meta=(AllowPrivateAccess=true))
	UDataAsset_InputConfig* InputConfigDataAsset;

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);

	FVector2D SwitchDirection=FVector2D::ZeroVector;
	float SwitchTotalTime=0.f;
	
	void Input_ToggleWalk();
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	
#pragma endregion

public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent()const {return HeroCombatComponent;}
};
