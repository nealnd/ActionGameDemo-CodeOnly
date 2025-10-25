// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifies/AttackCollision.h"
#if WITH_EDITOR
#include "AnimPreviewInstance.h"
#endif
#include "Components/Combat/AttackCollisionComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Library/ActionFunctionLibrary.h"

void UAttackCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
                                   const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	MeshComponent = MeshComp;
	if (AttackCollisionComponent)
	{
		
		AttackCollisionComponent->DestroyComponent();
		AttackCollisionComponent = nullptr;
	}
	AttackCollisionComponent = NewObject<UAttackCollisionComponent>(MeshComp->GetOwner());
	AttackCollisionComponent->AttackCollisionParameters = AttackCollisionParameters;
	AttackCollisionComponent->HitParameters= HitParameters;
	AttackCollisionComponent->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	AttackCollisionComponent->RegisterComponent();
	if (UPawnCombatComponent* CombatComponent = UActionFunctionLibrary::NativeGetPawnCombatComponentFromActor(MeshComp->GetOwner()))
	{
		CombatComponent->RegisterSpawnedAttackCollisionComponent(AttackCollisionComponent);
	}


#if WITH_EDITOR
	if (GIsEditor && !GIsPlayInEditorWorld)
	{
		AttackCollisionComponent->SetComponentTickEnabled(false);
	}
#endif
}


void UAttackCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                 const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
#if WITH_EDITOR
	if (UAnimPreviewInstance* PreviewInstance = Cast<UAnimPreviewInstance>(MeshComp->GetAnimInstance()))
	{
		const float CurrentTime = PreviewInstance->GetCurrentTime();
		const float BeginTime = EventReference.GetNotify()->GetTriggerTime();
		const float EndTime = EventReference.GetNotify()->GetEndTriggerTime();
		
		if ((CurrentTime < BeginTime || CurrentTime > EndTime) && AttackCollisionComponent)
		{
			AttackCollisionComponent->DestroyComponent();
			AttackCollisionComponent = nullptr;
		}
		return; 
	}
#endif
	if (UPawnCombatComponent* CombatComponent = UActionFunctionLibrary::NativeGetPawnCombatComponentFromActor(MeshComp->GetOwner()))
	{
		CombatComponent->RemoveAttackCollisionComponent(AttackCollisionComponent);
	}
	if (AttackCollisionComponent)
	{
		AttackCollisionComponent->DestroyComponent();
		AttackCollisionComponent = nullptr;
	}
}

#if WITH_EDITOR
void UAttackCollision::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);


	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	const FName MemberPropertyName = PropertyChangedEvent.MemberProperty
		                                 ? PropertyChangedEvent.MemberProperty->GetFName()
		                                 : NAME_None;

	if (PropertyName == GET_MEMBER_NAME_CHECKED(UAttackCollision, AttackCollisionParameters) ||
		MemberPropertyName == GET_MEMBER_NAME_CHECKED(UAttackCollision, AttackCollisionParameters))
	{
		if (AttackCollisionComponent && MeshComponent)
		{
			AttackCollisionComponent->SetAttackCollisionComponent(AttackCollisionParameters, MeshComponent);
		}
	}
}
#endif
