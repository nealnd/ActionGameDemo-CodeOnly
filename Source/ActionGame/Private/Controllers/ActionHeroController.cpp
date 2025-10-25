// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGame/Public/Controllers/ActionHeroController.h"

AActionHeroController::AActionHeroController()
{
	HeroTeamId=FGenericTeamId(0);
}
FGenericTeamId AActionHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}

