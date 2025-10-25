// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/SkillSelectorInterface.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "ActionAIController.generated.h"


class UActionAbilitySystemComponent;
struct FAIStimulus;
class UAISenseConfig_Sight;

UCLASS()
class ACTIONGAME_API AActionAIController : public AAIController,public ISkillSelectorInterface
{
	GENERATED_BODY()

public:

	AActionAIController(const FObjectInitializer& ObjectInitializer);

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION(BlueprintCallable)
	virtual USkillSelectorComponent* GetSkillSelectorComponent()const override;

	UActionAbilitySystemComponent* GetActionASCFromAIController() const;
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SkillSelector")
	USkillSelectorComponent* SkillSelectorComponent;

private:
	UPROPERTY(EditDefaultsOnly,Category="Detour Crowd Avoidance Config")
	bool bEnableDetourCrowdAvoidance=true;

	UPROPERTY(EditDefaultsOnly,Category="Detour Crowd Avoidance Config",meta=(EditCondition="bEnableDetourCrowdAvoidance",UIMin="1",UIMax="4"))
	int32 DetourCrowdAvoidanceQuality=4;

	UPROPERTY(EditDefaultsOnly,Category="Detour Crowd Avoidance Config",meta=(EditCondition="bEnableDetourCrowdAvoidance"))
	float CrowdCollisionQueryRange=600.f;
};
