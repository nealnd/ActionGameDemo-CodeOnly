// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AttackDirectionAdjuster.h"

#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Characters/ActionHeroCharacter.h"
#include "Controllers/ActionHeroController.h"
#include "Library/ActionFunctionLibrary.h"

UAttackDirectionAdjuster::UAttackDirectionAdjuster(const FObjectInitializer& ObjectInitializer):
	Super(ObjectInitializer)
{
	if (URootMotionModifier_Warp* Modifier = Cast<URootMotionModifier_Warp>(RootMotionModifier))
	{
		Modifier->WarpTargetName = FName("AdjustAttackDirection");
		Modifier->bWarpTranslation = false;
		Modifier->bWarpRotation = true;
	}
#if WITH_EDITOR
	bShouldFireInEditor=false;
#endif
}

void UAttackDirectionAdjuster::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                           float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	InputDirection = FVector::ZeroVector;
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = Cast<AActionHeroCharacter>(MeshComp->GetOwner());
		
	}
	
	if (!CachedHeroController.IsValid())
	{
		CachedHeroController=MeshComp->GetOwner()->GetInstigatorController<AActionHeroController>();
	}

	check(CachedHeroCharacter.IsValid());
	MoveInputDelegateHandle = CachedHeroCharacter->OnMoveInputTriggered.AddUObject(this, &UAttackDirectionAdjuster::OnMoveInputTriggered);
	MotionWarpingComponent = CachedHeroCharacter->GetMotionWarpingComponent();
	check(MotionWarpingComponent.IsValid());
	FRotator InitialRotation = InputDirection.IsNearlyZero()
											   ? CachedHeroCharacter->GetActorRotation()
											   : InputDirection.Rotation();
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
		FName("AdjustAttackDirection"), CachedHeroCharacter->GetActorLocation(), InitialRotation);
}

void UAttackDirectionAdjuster::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	
	if (UActionFunctionLibrary::NativeDoesActorHaveTag(MeshComp->GetOwner(), ActionGameplayTags::Player_Status_TargetLock))
	{
		FRotator AttackRotation =CachedHeroController->GetControlRotation();
		AttackRotation.Pitch=0.0f;
		AttackRotation.Roll=0.0f;
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
			FName("AdjustAttackDirection"), MeshComp->GetOwner()->GetActorLocation(),
			AttackRotation);
	}
	
	else if (!InputDirection.IsNearlyZero())
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
			FName("AdjustAttackDirection"), MeshComp->GetOwner()->GetActorLocation(),
			InputDirection.Rotation());
	}
}

void UAttackDirectionAdjuster::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	InputDirection = FVector::ZeroVector;
	if (CachedHeroCharacter.IsValid()&&MoveInputDelegateHandle.IsValid())
	{
		CachedHeroCharacter->OnMoveInputTriggered.Remove(MoveInputDelegateHandle);
	}
	if (MotionWarpingComponent.IsValid())
	{
		MotionWarpingComponent->RemoveWarpTarget(FName("AdjustAttackDirection"));
	}
	CachedHeroCharacter = nullptr;
	CachedHeroController = nullptr;
}

void UAttackDirectionAdjuster::OnMoveInputTriggered(const FVector& NewInputDirection)
{
	InputDirection = NewInputDirection;
}
