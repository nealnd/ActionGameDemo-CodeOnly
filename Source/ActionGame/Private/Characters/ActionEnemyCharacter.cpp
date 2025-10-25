// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionEnemyCharacter.h"

#include "Components/WidgetComponent.h"
#include "Components/Combat/EnemyCombatComponent.h"
#include "Components/Combat/HitReactionComponent.h"
#include "Components/UI/EnemyUIComponent.h"
#include "DataAssets/StartUpDataBase/DataAsset_StartUpDataBase.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/ActionWidgetBase.h"


AActionEnemyCharacter::AActionEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);

	EnemyCombatComponent = CreateDefaultSubobject<UEnemyCombatComponent>("EnemyCombatComponent");
	EnemyUIComponent = CreateDefaultSubobject<UEnemyUIComponent>("EnemyUIComponent");
	EnemyHealthWidgetComponent=CreateDefaultSubobject<UWidgetComponent>("EnemyHealthWidgetComponent");
	EnemyHealthWidgetComponent->SetupAttachment(GetMesh());
}

UPawnCombatComponent* AActionEnemyCharacter::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}

UPawnUIComponent* AActionEnemyCharacter::GetPawnUIComponent() const
{
	return EnemyUIComponent;
}

UEnemyUIComponent* AActionEnemyCharacter::GetEnemyUIComponent() const
{
	return EnemyUIComponent;
}

void AActionEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UActionWidgetBase* EnemyHealthWidget = Cast<UActionWidgetBase> (EnemyHealthWidgetComponent->GetUserWidgetObject()))
	{
		EnemyHealthWidget->InitEnemyCreatedWidget(this);
	}
}

void AActionEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitEnemyStartUpData();
}

void AActionEnemyCharacter::InitEnemyStartUpData()
{
	if (CharacterStartUpData.IsNull())
	{
		return;
	}

	UAssetManager::GetStreamableManager().
		RequestAsyncLoad(CharacterStartUpData.ToSoftObjectPath()
		                 , FStreamableDelegate::CreateLambda(
			                 [this]()
			                 {
				                 if (UDataAsset_StartUpDataBase* LoadedData =
					                 CharacterStartUpData.Get())
				                 {
					                 LoadedData->GiveToAbilitySystemComponent(
						                 AbilitySystemComponent);
				                 	HitReactionComponent->SetCurrentHitReactions(LoadedData->GetHitReactionAbilities(),true);
				                 }
			                 }));
}
