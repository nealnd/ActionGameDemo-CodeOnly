// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "SkillJumpTo.generated.h"


class UActionAbilitySystemComponent;
class UActionHeroGameplayAbility;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API USkillJumpTo : public UAnimNotifyState
{
	GENERATED_BODY()
public:

	USkillJumpTo();
	
	UPROPERTY(EditAnywhere)
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActionHeroGameplayAbility>TargetAbilityClass;

	UPROPERTY()
	FGameplayEventData EventData;

	virtual  void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual  void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	TWeakObjectPtr<UActionAbilitySystemComponent>ASC;
};
