// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckCurrentAbilityToExecute.generated.h"

class UGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UBTDecorator_CheckCurrentAbilityToExecute : public UBTDecorator
{
	GENERATED_BODY()
public:

	UBTDecorator_CheckCurrentAbilityToExecute(const FObjectInitializer& ObjectInitializer);

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;



	
protected:
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	TSubclassOf<UGameplayAbility> TargetAbilityClass;
	
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector AbilityClassKey;

};
