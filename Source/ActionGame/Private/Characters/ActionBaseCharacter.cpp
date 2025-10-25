// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionGame/Public/Characters/ActionBaseCharacter.h"

#include "MotionWarpingComponent.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/ActionAttributeSet.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Components/Combat/AttackCollisionComponent.h"
#include "Components/Combat/HitReactionComponent.h"
#include "Components/Combat/PawnCombatComponent.h"
#include "Components/Combat/TargetLockComponent.h"
#include "Components/UI/PawnUIComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Library/ActionFunctionLibrary.h"

#include "States/ActionHeroState.h"


// Sets default values
AActionBaseCharacter::AActionBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick =false;
	// PrimaryActorTick.bStartWithTickEnabled=false;
	//GetMesh()->bReceivesDecals=false;

	AbilitySystemComponent = CreateDefaultSubobject<UActionAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UActionAttributeSet>(TEXT("AttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	HitReactionComponent = CreateDefaultSubobject<UHitReactionComponent>(TEXT("HitReactionComponent"));
	TargetLockComponent = CreateDefaultSubobject<UTargetLockComponent>(TEXT("TargetLockComponent"));
}

UAbilitySystemComponent* AActionBaseCharacter::GetAbilitySystemComponent() const
{
	return GetActionAbilitySystemComponent();
}

UPawnCombatComponent* AActionBaseCharacter::GetPawnCombatComponent() const
{
	return nullptr;
}

UPawnUIComponent* AActionBaseCharacter::GetPawnUIComponent() const
{
	return nullptr;
}

UHitReactionComponent* AActionBaseCharacter::GetHitReactionComponent() const
{
	return HitReactionComponent;
}

UTargetLockComponent* AActionBaseCharacter::GetTargetLockComponent() const
{
	return TargetLockComponent;
}


void AActionBaseCharacter::OnCharacterHitReact(const FGameplayEventData* GameplayEventData) const
{
	
	TSubclassOf<UGameplayAbility> AbilityToExecute;
	FActionHitResultInfo HitResultInfo;
	GetPawnCombatComponent()->GetHitResultInfo(HitResultInfo);
	const AActor* Attacker = HitResultInfo.Attacker;
	HitReactionComponent->OnCharacterHitReact(Attacker, HitResultInfo, AbilityToExecute);
	if (AbilityToExecute)
	{
		AbilitySystemComponent->TryActivateAbilityByClass(AbilityToExecute);
	}
}

void AActionBaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PC = Cast<APlayerController>(NewController))
	{
		if (AActionHeroState* ActionPS = PC->GetPlayerState<AActionHeroState>())
		{
			AbilitySystemComponent = Cast<UActionAbilitySystemComponent>(ActionPS->GetActionAbilitySystemComponent());
			AttributeSet = ActionPS->GetActionAttributeSet();
			AbilitySystemComponent->AddAttributeSetSubobject<UActionAttributeSet>(AttributeSet);
			AbilitySystemComponent->InitAbilityActorInfo(ActionPS, this);
		}
	}
	else
	{
		AbilitySystemComponent->AddAttributeSetSubobject<UActionAttributeSet>(AttributeSet);
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentHealthAttribute()).
	                        AddUObject(this, &AActionBaseCharacter::OnCurrentHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(
		this, &AActionBaseCharacter::OnMaxHealthChanged);

	CurrentGaitSettings = DefaultGaitSettings;

	AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(
		ActionGameplayTags::Shared_Event_HitReact).AddUObject(this, &ThisClass::OnCharacterHitReact);

	ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Character start up data is null"));
}

void AActionBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AActionBaseCharacter::OnCurrentHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GetPawnUIComponent()->OnHealthChanged.Broadcast(OnAttributeChangeData.NewValue / AttributeSet->GetMaxHealth());
}

void AActionBaseCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	GetPawnUIComponent()->OnHealthChanged.Broadcast(AttributeSet->GetCurrentHealth() / OnAttributeChangeData.NewValue);
}

void AActionBaseCharacter::UpdateGait(EGait DesiredGait)
{
	CurrentGait = DesiredGait;
	checkf(!CurrentGaitSettings.IsEmpty(), TEXT("Gait settings is null, can not proceed with updating gait"));
	FGaitSettings* GaitSetting = CurrentGaitSettings.Find(CurrentGait);
	checkf(GaitSetting != nullptr,
	       TEXT("Gait setting for the current gait is not found, can not proceed with updating gait"));
	checkf(GaitSetting->MaxWalkSpeed>0, TEXT("Gait settings is not valid, can not proceed with updating gait"));

	GetCharacterMovement()->MaxWalkSpeed = GaitSetting->MaxWalkSpeed;
	GetCharacterMovement()->MaxAcceleration = GaitSetting->MaxAcceleration;
	GetCharacterMovement()->BrakingDecelerationWalking = GaitSetting->BrakingDeceleration;
	GetCharacterMovement()->BrakingFrictionFactor = GaitSetting->BrakingFrictionFactor;
	GetCharacterMovement()->bUseSeparateBrakingFriction = GaitSetting->bUseSeparateBrakingFriction;
	GetCharacterMovement()->BrakingFriction = GaitSetting->BrakingFriction;
}

void AActionBaseCharacter::ChangeGaitSettings(const TMap<EGait, FGaitSettings>& InGaitSettings)
{
	CurrentGaitSettings = InGaitSettings;
}

UHitReactionComponent* AActionBaseCharacter::GetHitReactionComponent()
{
	return HitReactionComponent;
}

UTargetLockComponent* AActionBaseCharacter::GetTargetLockComponent()
{
	return TargetLockComponent;
}
