// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumLibrary.h"
#include "GameplayTagContainer.h"
#include "StructLibrary.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ActionFunctionLibrary.generated.h"

class UActionGameInstance;
class UHitReactionComponent;
class UPawnCombatComponent;
class UActionAbilitySystemComponent;



/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UActionAbilitySystemComponent* NativeGetActionASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor,FGameplayTag TagToAdd,bool bShouldReplicate=false);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary")
	static void AddGameplayTagsToActorIfNone(AActor* InActor,FGameplayTagContainer TagsToAdd,bool bShouldReplicate=false);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary")
	static void RemoveGameplayTagToActor(AActor* InActor,FGameplayTag TagToRemove,bool bShouldReplicate=false);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary")
	static void RemoveGameplayTagsToActor(AActor* InActor,FGameplayTagContainer TagsToRemove,bool bShouldReplicate=false);

	static bool NativeDoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary",meta=(DisplayName="Does Actor Have Tag",ExpandEnumAsExecs="OutConfirmType"))
	static void BP_DoesActorHaveTag(AActor* InActor,FGameplayTag TagToCheck,EActionConfirmType& OutConfirmType);
	
	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary",meta=(DisplayName="Get Pawn Combat Component From Actor",ExpandEnumAsExecs="OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor,EActionValidType& OutValidType);

	static UHitReactionComponent* NativeGetHitReactionComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary",meta=(DisplayName="Get Hit Reaction Component From Actor",ExpandEnumAsExecs="OutValidType"))
	static UHitReactionComponent* BP_GetHitReactionComponentFromActor(AActor* InActor,EActionValidType& OutValidType);

	UFUNCTION(BlueprintPure,Category="Action|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QuerryPawn,APawn* TargetPawn);

	UFUNCTION(BlueprintPure,Category="Action|FunctionLibrary")
	static bool IsValidBlockAngle(AActor* InAttacker,AActor* InDefender);

	UFUNCTION(BlueprintPure,Category="Action|FunctionLibrary",meta=(WorldContext="WorldContextObject"))
	static UActionGameInstance* GetActionGameInstance(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable,Category="Action|FunctionLibrary",meta=(WorldContext="WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject,EActionInputMode InInputMode);
};
