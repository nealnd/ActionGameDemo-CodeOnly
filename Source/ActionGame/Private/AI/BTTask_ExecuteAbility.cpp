// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_ExecuteAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AIController.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/ActionEnemyGameplayAbility.h"
#include "ActionGame/ActionGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Class.h"
#include "Components/Combat/SkillSelectorComponent.h"
#include "Controllers/ActionAIController.h"
#include "Library/ActionFunctionLibrary.h"


UBTTask_ExecuteAbility::UBTTask_ExecuteAbility(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	NodeName = TEXT("Execute Ability");
	INIT_TASK_NODE_NOTIFY_FLAGS();
	AbilityToExecute.AddClassFilter(this,GET_MEMBER_NAME_CHECKED(ThisClass, AbilityToExecute),
	                                UGameplayAbility::StaticClass());
	bCreateNodeInstance = true;
}

void UBTTask_ExecuteAbility::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		AbilityToExecute.ResolveSelectedKey(*BBAsset);
	}
}


EBTNodeResult::Type UBTTask_ExecuteAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!CachedAIController.IsValid())
	{
		CachedAIController=Cast<AActionAIController>(OwnerComp.GetAIOwner());
	}
	UClass* AbilityClass;
	if (bUseAbilityInBlackboard)
	{
		AbilityClass = OwnerComp.GetBlackboardComponent()->GetValueAsClass(AbilityToExecute.SelectedKeyName);
	}
	else
	{
		AbilityClass = AbilityToExecuteClass;
	}

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!AbilityClass || !OwningPawn)
	{
		return EBTNodeResult::Failed;
	}

	UAbilitySystemComponent* ASC =UActionFunctionLibrary::NativeGetActionASCFromActor(OwningPawn);
	if (!ASC)
	{
		return EBTNodeResult::Failed;
	}


	if (ASC->TryActivateAbilityByClass(AbilityClass))
	{
		CurrentAbilityClass = AbilityClass;
		CachedAIController->GetSkillSelectorComponent()->OnExecutingAbility.ExecuteIfBound(AbilityClass);
		 DelegateHandle = ASC->RegisterGameplayTagEvent(
			ActionGameplayTags::Enemy_Event_FinishTaskAhead,
			EGameplayTagEventType::NewOrRemoved
		).AddLambda([this, &OwnerComp](const FGameplayTag Tag, int32 Count)
		{
			//Debug::Print("Finish Earlier");
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		});
		FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromClass(AbilityClass);
		if (Spec && Spec->IsActive())
		{
			/*ASC->FindAbilitySpecFromClass(AbilityClass)->Ability get the CDO,not the instance*/
			if (UGameplayAbility* Instance = Spec->GetPrimaryInstance())
			{
				Instance->OnGameplayAbilityEnded.AddLambda(
					[this, &OwnerComp,ASC](UGameplayAbility* Ability)
					{
						CurrentAbilityClass = nullptr;
						ASC->UnregisterGameplayTagEvent(DelegateHandle, ActionGameplayTags::Enemy_Event_FinishTaskAhead,
						                                EGameplayTagEventType::NewOrRemoved);
						ASC->RemoveLooseGameplayTag(ActionGameplayTags::Enemy_Event_FinishTaskAhead);
						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
					});
			}
		}

		return EBTNodeResult::InProgress;
	}

	return
		EBTNodeResult::Failed;
}

void UBTTask_ExecuteAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	CurrentAbilityClass=nullptr;
	if (OwnerComp.GetAIOwner()->GetPawn())
	{
		UAbilitySystemComponent* ASC =UActionFunctionLibrary::NativeGetActionASCFromActor(OwnerComp.GetAIOwner()->GetPawn());
		if (ASC)
		{
			ASC->UnregisterGameplayTagEvent(DelegateHandle, ActionGameplayTags::Enemy_Event_FinishTaskAhead,
											EGameplayTagEventType::NewOrRemoved);
		}
	}
	
}

void UBTTask_ExecuteAbility::DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);
	const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	if (BlackboardComp)
	{
		const FString KeyValue = CurrentAbilityClass? CurrentAbilityClass->GetName() : TEXT("None");
		
		Values.Add(FString::Printf(TEXT("Execute Ability: %s"), *KeyValue));
	}
}

FString UBTTask_ExecuteAbility::GetStaticDescription() const
{
	if (AbilityToExecuteClass)
	{
		const FString KeyDesc=bUseAbilityInBlackboard ?  "Random Ability":AbilityToExecuteClass->GetName() ;
		return FString::Printf(TEXT("Ability To Execute: %s"), *KeyDesc);
	}
	return FString::Printf(TEXT("Ability To Execute: None"));
}
