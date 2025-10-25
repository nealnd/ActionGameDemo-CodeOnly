// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_CheckCurrentAbilityToExecute.h"

#include "Abilities/GameplayAbility.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckCurrentAbilityToExecute::UBTDecorator_CheckCurrentAbilityToExecute(
	const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	NodeName = TEXT("CheckCurrentAbilityToExecute");
	INIT_DECORATOR_NODE_NOTIFY_FLAGS();
	AbilityClassKey.AddClassFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass,AbilityClassKey),UGameplayAbility::StaticClass());
	
}

bool UBTDecorator_CheckCurrentAbilityToExecute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory) const
{
	if (!TargetAbilityClass)
	{
		return false;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return false;
	UClass* ClassInBB = BBComp->GetValueAsClass(AbilityClassKey.SelectedKeyName);

	return ClassInBB == TargetAbilityClass;
}

void UBTDecorator_CheckCurrentAbilityToExecute::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		AbilityClassKey.ResolveSelectedKey(*BBAsset);
	}
	
}

FString UBTDecorator_CheckCurrentAbilityToExecute::GetStaticDescription() const
{
	const FString KeyDesc=TargetAbilityClass->GetName() ;
	return FString::Printf(TEXT("%s is equal to %s"),*AbilityClassKey.SelectedKeyName.ToString(),*KeyDesc);
}


