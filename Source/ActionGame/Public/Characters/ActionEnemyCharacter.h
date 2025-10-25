// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionBaseCharacter.h"
#include "ActionEnemyCharacter.generated.h"

class UWidgetComponent;
class UEnemyUIComponent;
class UEnemyCombatComponent;

UCLASS()
class ACTIONGAME_API AActionEnemyCharacter : public AActionBaseCharacter
{
	GENERATED_BODY()

public:
	AActionEnemyCharacter();

	virtual UPawnCombatComponent* GetPawnCombatComponent()const override;
	virtual UPawnUIComponent* GetPawnUIComponent()const override;
	virtual UEnemyUIComponent* GetEnemyUIComponent()const override;

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Combat")
	UEnemyCombatComponent* EnemyCombatComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="UI")
	UEnemyUIComponent* EnemyUIComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="UI")
	UWidgetComponent* EnemyHealthWidgetComponent;

private:
	void InitEnemyStartUpData();

	
public:
	FORCEINLINE UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo() const {return EnemyCombatComponent;}

};
