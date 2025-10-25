// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/SkillJumpTo.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h" 
#include "AbilitySystem/Abilities/ActionHeroGameplayAbility.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Library/ActionFunctionLibrary.h"
#include "InputMappingContext.h"

USkillJumpTo::USkillJumpTo(): InputMappingContext(nullptr)
{
#if WITH_EDITOR
	bShouldFireInEditor = false;
#endif
}

void USkillJumpTo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (!ASC.IsValid())
	{
		ASC = UActionFunctionLibrary::NativeGetActionASCFromActor(MeshComp->GetOwner());
	}
	EventData.OptionalObject = InputMappingContext;
	EventData.OptionalObject2=TargetAbilityClass.Get();
	ASC->HandleGameplayEvent(ActionGameplayTags::Player_Event_EnableSkillSwitch,&EventData);
}

void USkillJumpTo::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
                              const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	if (!ASC.IsValid())
	{
		ASC = UActionFunctionLibrary::NativeGetActionASCFromActor(MeshComp->GetOwner());
	}
	if (!ASC->HasMatchingGameplayTag(ActionGameplayTags::Player_Status_EnableSkillSwitch))
	{
		ASC->HandleGameplayEvent(ActionGameplayTags::Player_Event_EnableSkillSwitch,&EventData);
	}
}

void USkillJumpTo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                             const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	if (ASC.IsValid())
	{
		ASC->HandleGameplayEvent(ActionGameplayTags::Player_Event_DisableSkillSwitch,&EventData);
	}
}
