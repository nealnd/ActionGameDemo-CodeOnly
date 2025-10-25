// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGame/Public/GameModes/ActionGameModeBase.h"

void AActionGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AActionGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AActionGameModeBase::SetCurrentGameModeState(EActionGameModeState NewState)
{
	CurrentGameModeState = NewState;
	OnGameModeStateChanged.Broadcast(CurrentGameModeState);
}
