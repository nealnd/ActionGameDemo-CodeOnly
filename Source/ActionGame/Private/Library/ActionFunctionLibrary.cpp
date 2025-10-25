// Fill out your copyright notice in the Description page of Project Settings.


#include "Library/ActionFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "Interfaces/HitReactionInterface.h"
#include "Interfaces/PawnCombatInterface.h"
#include "Library/EnumLibrary.h"
#include "ActionGameInstance.h"

UActionAbilitySystemComponent* UActionFunctionLibrary::NativeGetActionASCFromActor(AActor* InActor)
{
	check(InActor);

	return CastChecked<UActionAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UActionFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd, bool bShouldReplicate)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionASCFromActor(InActor);
	if (ASC && !ASC->HasMatchingGameplayTag(TagToAdd))
	{
		if (bShouldReplicate)
		{
			ASC->AddReplicatedLooseGameplayTag(TagToAdd);
		}
		else
		{
			ASC->AddLooseGameplayTag(TagToAdd);
		}
	}
}

void UActionFunctionLibrary::AddGameplayTagsToActorIfNone(AActor* InActor, FGameplayTagContainer TagsToAdd,
                                                          bool bShouldReplicate)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionASCFromActor(InActor);
	if (ASC && !TagsToAdd.IsEmpty())
	{
		for (auto& Tag : TagsToAdd)
		{
			if (!ASC->HasMatchingGameplayTag(Tag))
			{
				if (bShouldReplicate)
				{
					ASC->AddReplicatedLooseGameplayTag(Tag);
				}
				else
				{
					ASC->AddLooseGameplayTag(Tag);
				}
			}
		}
	}
}


void UActionFunctionLibrary::RemoveGameplayTagToActor(AActor* InActor, FGameplayTag TagToRemove, bool bShouldReplicate)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionASCFromActor(InActor);
	if (ASC && ASC->HasMatchingGameplayTag(TagToRemove))
	{
		if (bShouldReplicate)
		{
			ASC->RemoveReplicatedLooseGameplayTag(TagToRemove);
		}
		else
		{
			ASC->RemoveLooseGameplayTag(TagToRemove);
		}
	}
}

void UActionFunctionLibrary::RemoveGameplayTagsToActor(AActor* InActor, FGameplayTagContainer TagsToRemove,
                                                       bool bShouldReplicate)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionASCFromActor(InActor);
	if (ASC && !TagsToRemove.IsEmpty())
	{
		for (auto& Tag : TagsToRemove)
		{
			if (ASC->HasMatchingGameplayTag(Tag))
			{
				if (bShouldReplicate)
				{
					ASC->RemoveReplicatedLooseGameplayTag(Tag);
				}
				else
				{
					ASC->RemoveLooseGameplayTag(Tag);
				}
			}
		}
	}
}

bool UActionFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
	UActionAbilitySystemComponent* ASC = NativeGetActionASCFromActor(InActor);
	return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UActionFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck,
                                                 EActionConfirmType& OutConfirmType)
{
	OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EActionConfirmType::Yes : EActionConfirmType::No;
}

UPawnCombatComponent* UActionFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IPawnCombatInterface* PawnCombatInterface = Cast<IPawnCombatInterface>(InActor))
	{
		return PawnCombatInterface->GetPawnCombatComponent();
	}
	return nullptr;
}

UPawnCombatComponent* UActionFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor,
	EActionValidType& OutValidType)
{
	UPawnCombatComponent* CombatComponent = NativeGetPawnCombatComponentFromActor(InActor);
	OutValidType = CombatComponent ? EActionValidType::Valid : EActionValidType::Invalid;
	return CombatComponent;
}

UHitReactionComponent* UActionFunctionLibrary::NativeGetHitReactionComponentFromActor(AActor* InActor)
{
	check(InActor);
	if (IHitReactionInterface* HitReactionInterface = Cast<IHitReactionInterface>(InActor))
	{
		return HitReactionInterface->GetHitReactionComponent();
	}
	return nullptr;
}

UHitReactionComponent* UActionFunctionLibrary::BP_GetHitReactionComponentFromActor(AActor* InActor,
	EActionValidType& OutValidType)
{
	UHitReactionComponent* HitReactionComponent = NativeGetHitReactionComponentFromActor(InActor);
	OutValidType = HitReactionComponent ? EActionValidType::Valid : EActionValidType::Invalid;
	return HitReactionComponent;
}

bool UActionFunctionLibrary::IsTargetPawnHostile(APawn* QuerryPawn, APawn* TargetPawn)
{
	check(QuerryPawn&&TargetPawn);

	IGenericTeamAgentInterface* QuerryTeamAgent = Cast<IGenericTeamAgentInterface>(QuerryPawn->GetController());
	IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());
	if (QuerryTeamAgent && TargetTeamAgent)
	{
		return QuerryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();
	}
	return false;
}

bool UActionFunctionLibrary::IsValidBlockAngle(AActor* InAttacker, AActor* InDefender)
{
	const FVector DefenderForward = InDefender->GetActorForwardVector();
	FVector HitDirection = (InAttacker->GetActorLocation() - InDefender->GetActorLocation()).GetSafeNormal();
	HitDirection.Z = 0.f;
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(DefenderForward, HitDirection)));
	return Angle <= 75.f;
}

UActionGameInstance* UActionFunctionLibrary::GetActionGameInstance(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->
			GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			return World->GetGameInstance<UActionGameInstance>();
		}
	}
	return nullptr;
}

void UActionFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EActionInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;
	if (GEngine)
	{
		if (UWorld* World = GEngine->
			GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}

	if (!PlayerController)
	{
		return;
	}

	FInputModeGameOnly GameOnlyMode;
	FInputModeGameAndUI UIOnlyMode;

	switch (InInputMode)
	{
	case EActionInputMode::GameOnly:
		PlayerController->SetInputMode(GameOnlyMode);
		PlayerController->bShowMouseCursor = false;
		break;
	case EActionInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnlyMode);
		PlayerController->bShowMouseCursor = true;

		break;
	default:
		break;
	}
}
