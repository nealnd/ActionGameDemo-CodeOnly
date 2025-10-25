// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ActionHeroCharacter.h"

#include "AbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionGame/ActionGameplayTags.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"

#include "ActionGame/DebugHelper.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "Components/Input/ActionInputComponent.h"
#include "DataAssets/StartUpDataBase/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/ActionAbilitySystemComponent.h"
#include "AbilitySystem/ActionAttributeSet.h"
#include "Components/Combat/HitReactionComponent.h"
#include "Components/Combat/TargetLockComponent.h"
#include "Components/UI/HeroUIComponent.h"

AActionHeroCharacter::AActionHeroCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>("FollowCamera");
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>("HeroCombatComponent");
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>("HeroUIComponent");
}

UPawnCombatComponent* AActionHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AActionHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AActionHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

void AActionHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AActionHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetCurrentSpiritAttribute()).
	                        AddUObject(this, &AActionHeroCharacter::OnCurrentSpiritChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxSpiritAttribute()).AddUObject(
		this, &AActionHeroCharacter::OnMaxSpiritChanged);
	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(AbilitySystemComponent);
			HitReactionComponent->SetCurrentHitReactions(LoadedData->GetHitReactionAbilities(), true);
		}
	}
}

void AActionHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Input config data asset is null, can not proceed with binding"));
	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
		UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UActionInputComponent* ActionInputComponent = CastChecked<UActionInputComponent>(PlayerInputComponent);
	ActionInputComponent->BindNativeInputAction(InputConfigDataAsset, ActionGameplayTags::InputTag_Move,
	                                            ETriggerEvent::Triggered, this, &AActionHeroCharacter::Input_Move);
	ActionInputComponent->BindNativeInputAction(InputConfigDataAsset, ActionGameplayTags::InputTag_Look,
	                                            ETriggerEvent::Triggered, this, &AActionHeroCharacter::Input_Look);
	ActionInputComponent->BindNativeInputAction(InputConfigDataAsset, ActionGameplayTags::InputTag_ToggleWalk,
	                                            ETriggerEvent::Started, this, &AActionHeroCharacter::Input_ToggleWalk);
	ActionInputComponent->BindNativeInputAction(InputConfigDataAsset, ActionGameplayTags::InputTag_SwitchTarget,
	                                            ETriggerEvent::Triggered, this,
	                                            &AActionHeroCharacter::Input_SwitchTargetTriggered);
	ActionInputComponent->BindNativeInputAction(InputConfigDataAsset, ActionGameplayTags::InputTag_SwitchTarget,
	                                            ETriggerEvent::Completed, this,
	                                            &AActionHeroCharacter::Input_SwitchTargetCompleted);
	ActionInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed,
	                                             &ThisClass::Input_AbilityInputReleased);
}

void AActionHeroCharacter::OnCurrentSpiritChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	HeroUIComponent->OnSpiritChanged.Broadcast(OnAttributeChangeData.NewValue / AttributeSet->GetMaxSpirit());
}

void AActionHeroCharacter::OnMaxSpiritChanged(const FOnAttributeChangeData& OnAttributeChangeData) const
{
	HeroUIComponent->OnSpiritChanged.Broadcast(AttributeSet->GetCurrentSpirit() / OnAttributeChangeData.NewValue);
}

void AActionHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	OnMoveInputTriggered.Broadcast(InputAxisVector.X * RightDirection + InputAxisVector.Y * ForwardDirection);

	AddMovementInput(ForwardDirection, InputAxisVector.Y);
	AddMovementInput(RightDirection, InputAxisVector.X);
}

void AActionHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputAxisVector.X);
	AddControllerPitchInput(InputAxisVector.Y);
}


void AActionHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection+=InputActionValue.Get<FVector2D>();
	SwitchTotalTime+=GetWorld()->GetDeltaSeconds();

	
}

void AActionHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	if (SwitchTotalTime<=1.f&&FMath::Abs(SwitchDirection.X)>=40.f)
	{
		GetTargetLockComponent()->SwitchTarget(SwitchDirection.X);
	}
	SwitchDirection=FVector2D::ZeroVector;
	SwitchTotalTime=0.f;
}

void AActionHeroCharacter::Input_ToggleWalk()
{
	if (CurrentGait == EGait::Walk)
	{
		UpdateGait(EGait::Run);
		Debug::Print(FString::SanitizeFloat(GetCharacterMovement()->MaxWalkSpeed));
	}
	else
	{
		UpdateGait(EGait::Walk);
	}
}

void AActionHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AActionHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	AbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
