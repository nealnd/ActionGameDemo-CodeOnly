// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/TrackTarget.h"

#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "RootMotionModifier.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/ActionBaseCharacter.h"
#include "Characters/ActionHeroCharacter.h"
#include "Kismet/KismetMathLibrary.h"

UTrackTarget::UTrackTarget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer),
                                                                         OffsetDistance(100.0f),
                                                                         MaxTrackingDistance(1500.0f),
                                                                         MaxWarpingDistance(1000.0f),
                                                                         MotionWarpingComponent(nullptr),
                                                                         bTracking(KeepTracking),
                                                                         TargetActor(nullptr),
                                                                         InitialLocation(FVector::ZeroVector),
                                                                         CurrentWarpingDistance(0.0f)
{
	
	if (URootMotionModifier_Warp* Modifier = Cast<URootMotionModifier_Warp>(RootMotionModifier))
	{
		Modifier->WarpTargetName = FName("TrackTarget");
		Modifier->bWarpTranslation = true;
		Modifier->bWarpRotation = true;
		Modifier->RotationType=EMotionWarpRotationType::Facing;
		Modifier->RotationMethod = EMotionWarpRotationMethod::ConstantRate;
	}
#if WITH_EDITOR
	bShouldFireInEditor=false;
#endif
	
}

void UTrackTarget::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	bTracking = KeepTracking;
	if (!SelfActor.IsValid())
	{
		SelfActor =  Cast<AActionBaseCharacter>(MeshComp->GetOwner());
	}
	check(SelfActor.IsValid());
	MotionWarpingComponent = SelfActor->GetMotionWarpingComponent();
	check(MotionWarpingComponent.IsValid());

	/*TODO: for hero character*/


	TargetActor =Cast<AActor>( UAIBlueprintHelperLibrary::GetAIController(SelfActor.Get())->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	check(TargetActor.IsValid());
	InitialLocation = SelfActor->GetActorLocation();
	if (TargetActor->GetDistanceTo(SelfActor.Get()) - OffsetDistance <= MaxTrackingDistance)
	{
		FVector Direction = TargetActor->GetActorLocation() - SelfActor->GetActorLocation();
		Direction.Z = 0;
		FVector WarpTargetLocation = TargetActor->GetActorLocation() - Direction.GetSafeNormal() *
			OffsetDistance;
		WarpTargetLocation.Z = 0;

		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
			FName("TrackTarget"), WarpTargetLocation,
			UKismetMathLibrary::FindLookAtRotation(SelfActor->GetActorLocation(),
												   TargetActor->GetActorLocation())
		);
	}
}

void UTrackTarget::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
	check(TargetActor.IsValid());
	CurrentWarpingDistance = FVector::DistXY(SelfActor->GetActorLocation(), InitialLocation);
	if (CurrentWarpingDistance > MaxWarpingDistance)
	{

		MotionWarpingComponent->RemoveWarpTarget("TrackTarget");
		bTracking = false;
	}
	if (bTracking)
	{
		if (TargetActor->GetDistanceTo(SelfActor.Get()) - OffsetDistance <= MaxTrackingDistance)
		{
			FVector Direction = TargetActor->GetActorLocation() - SelfActor->GetActorLocation();
			Direction.Z = 0;
			FVector WarpTargetLocation = TargetActor->GetActorLocation() - Direction.GetSafeNormal() *
				OffsetDistance;
			WarpTargetLocation.Z = 0;
			MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(
				FName("TrackTarget"), WarpTargetLocation, UKismetMathLibrary::FindLookAtRotation(
					SelfActor->GetActorLocation(),
					TargetActor->GetActorLocation()));
		}
	}
}

void UTrackTarget::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	InitialLocation = FVector::ZeroVector;
	CurrentWarpingDistance = 0.0f;
	if (MotionWarpingComponent.IsValid())
	{
		MotionWarpingComponent->RemoveWarpTarget(FName("TrackTarget"));
		MotionWarpingComponent = nullptr;
	}

}
