// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "ActionHeroController.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AActionHeroController : public APlayerController,public IGenericTeamAgentInterface
{
	GENERATED_BODY()
public:
	virtual FGenericTeamId GetGenericTeamId() const override;
	AActionHeroController();

private:
	FGenericTeamId HeroTeamId;
};
