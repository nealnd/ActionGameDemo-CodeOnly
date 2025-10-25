// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotifyState_MotionWarping.h"
#include "TrackTarget.generated.h"

class AActionBaseCharacter;
/**
 *  to simplify the configuration in Montage, mix the motionwarping with the track requirements
 */
UCLASS()
class ACTIONGAME_API UTrackTarget : public UAnimNotifyState_MotionWarping
{
	GENERATED_BODY()

public:
	UTrackTarget(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere)
	bool KeepTracking=true;

	/*track location would have a distance offset, TargetDistance-offsetDistance*/
	UPROPERTY(EditAnywhere)
	float OffsetDistance;

	/*max distance to track the target,exceed this distance will not track*/
	UPROPERTY(EditAnywhere)
	float MaxTrackingDistance;

	/*the max distance to warp,exceed this distance will not warp*/
	UPROPERTY(EditAnywhere)
	float MaxWarpingDistance;

	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void  NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	TWeakObjectPtr<UMotionWarpingComponent>MotionWarpingComponent;

	UPROPERTY()
	bool bTracking;

	UPROPERTY()
	TWeakObjectPtr<AActor>TargetActor;

	UPROPERTY()
	TWeakObjectPtr<AActionBaseCharacter>SelfActor;

	UPROPERTY()
	FVector InitialLocation;
	
	UPROPERTY()
	float CurrentWarpingDistance;
	

};
