// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PawnExtensionComponentBase.h"
#include "Library/StructLibrary.h"
#include "HitReactionComponent.generated.h"


class UGameplayAbility;
struct FGameplayEventData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UHitReactionComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable,Category="Action|Combat")
	void SetCurrentHitReactions(const TMap<int32,FActionHitReactionAbilityContainer> HitReactionAbilities,bool isDefaultHitReactions=false);

	UFUNCTION(BlueprintCallable,Category="Action|Combat")
	void SetDefaultHitReactions();

	void OnCharacterHitReact(const AActor* Attacker,const FActionHitResultInfo& HitResultInfo, TSubclassOf<UGameplayAbility>&AbilityToExecute);



private:
	UPROPERTY()
	TMap<int32,FActionHitReactionAbilityContainer>HitReactions;

	/*TODO:this is not good,no weapon could also seen as equip a weapon*/
	UPROPERTY()
	TMap<int32, FActionHitReactionAbilityContainer> DefaultHitReactions;
	
};

