// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/StructLibrary.h"
#include "SkillSelectorComponent.generated.h"

class UActionAbilitySystemComponent;
DECLARE_DELEGATE_OneParam(FOnExecutingAbility, UClass*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API USkillSelectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void InitializeComponent() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AI|SkillSelector")
	FActionAIWeightedSkillConfigContainer DefaultWeightedSkillConfigContainer;

	
	UFUNCTION(BlueprintCallable,Category="Action|AI")
	TSubclassOf<UGameplayAbility> RandomAppropriateAbility(float InDistance,float InAngle);

	FOnExecutingAbility OnExecutingAbility;

protected:
	void GetAppropriateAbilities(float InDistance,float InAngle,TMap<TSubclassOf<UGameplayAbility>,FActionAIWeightedSkillParameter>& InAppropriateAbilities);

	void RollbackWeightAfterCooldown(UClass* AbilityClass,float CooldownTime);

private:
	
	UPROPERTY()
	TWeakObjectPtr<UActionAbilitySystemComponent>ActionAbilitySystemComponent;

};
