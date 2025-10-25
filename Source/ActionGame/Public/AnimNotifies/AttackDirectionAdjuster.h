// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "AttackDirectionAdjuster.generated.h"

class AActionHeroController;
class AActionHeroCharacter;
/**
 * to simplify the configuration in Montage, mix the motionwarping with the input monitor
 */
UCLASS()
class ACTIONGAME_API UAttackDirectionAdjuster : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UAttackDirectionAdjuster(const FObjectInitializer& ObjectInitializer);

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void  NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;


private:
	
	UPROPERTY()
	FVector InputDirection=FVector::ZeroVector;

	UPROPERTY()
	TWeakObjectPtr<UMotionWarpingComponent>MotionWarpingComponent;

	UPROPERTY()
	TWeakObjectPtr<AActionHeroCharacter>CachedHeroCharacter;

	UPROPERTY()
	TWeakObjectPtr<AActionHeroController>CachedHeroController;
	
	UFUNCTION()
	void OnMoveInputTriggered(const FVector& NewInputDirection);

	FDelegateHandle MoveInputDelegateHandle;
	
};
