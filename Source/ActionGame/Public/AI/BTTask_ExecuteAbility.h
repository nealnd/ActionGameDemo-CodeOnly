// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ExecuteAbility.generated.h"

class UGameplayAbility;
class AActionAIController;
class UActionGameplayAbility;
/**
 * 
 */
UCLASS()
class ACTIONGAME_API UBTTask_ExecuteAbility : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTask_ExecuteAbility(const FObjectInitializer& ObjectInitializer);
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

public:
	UPROPERTY(EditAnywhere,Category="Ability")
	bool bUseAbilityInBlackboard=false;

	UPROPERTY(EditAnywhere,Category="Ability",meta=(EditCondition="bUseAbilityInBlackboard==false",EditConditionHides="true"))
	TSubclassOf<UGameplayAbility> AbilityToExecuteClass;
	
	UPROPERTY(EditAnywhere,Category="Ability",meta = ( EditCondition="bUseAbilityInBlackboard==true",EditConditionHides="true"))
	FBlackboardKeySelector AbilityToExecute;

private:
	TWeakObjectPtr<AActionAIController>CachedAIController;
	
	FDelegateHandle DelegateHandle;
	UPROPERTY()
	TSubclassOf<UGameplayAbility>CurrentAbilityClass;
};


