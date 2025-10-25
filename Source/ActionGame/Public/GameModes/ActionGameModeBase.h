// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Library/EnumLibrary.h"
#include "ActionGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameModeStateChanged, EActionGameModeState, CurrentState);
/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	void SetCurrentGameModeState(EActionGameModeState NewState);
	
	UPROPERTY()
	EActionGameModeState CurrentGameModeState;

	UPROPERTY(BlueprintAssignable,BlueprintCallable)
	FOnGameModeStateChanged OnGameModeStateChanged;
};
