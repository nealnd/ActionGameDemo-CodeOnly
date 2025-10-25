// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Library/StructLibrary.h"
#include "AttackCollision.generated.h"

class UAttackCollisionComponent;
struct FActionAttackCollisionParameters;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UAttackCollision : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitParameters HitParameters;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FActionAttackCollisionParameters> AttackCollisionParameters;


	
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	UPROPERTY()
	UAttackCollisionComponent* AttackCollisionComponent;

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;
};
