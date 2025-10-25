// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/TargetLockComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActionGame/ActionGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Characters/ActionHeroCharacter.h"
#include "Components/SizeBox.h"
#include "Controllers/ActionHeroController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Library/ActionFunctionLibrary.h"
#include "Widgets/ActionWidgetBase.h"


UTargetLockComponent::UTargetLockComponent(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}


void UTargetLockComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*TODO:if target distance is too far, we should also unlock,and we should find if there are any other appropriate targets*/
	if (!TargetActorToLock || UActionFunctionLibrary::NativeDoesActorHaveTag(
			TargetActorToLock, ActionGameplayTags::Shared_Status_Death) ||
		UActionFunctionLibrary::NativeDoesActorHaveTag(GetOwner(), ActionGameplayTags::Shared_Status_Death))
	{
		CleanUp();
		return;
	}
	
	UpdateCameraAndTargetLock();
}

void UTargetLockComponent::UpdateCameraAndTargetLock()
{
	if (CachedHeroCharacter.IsValid()&&CachedTargetLockInterface.IsValid()&&CachedHeroController.IsValid())
	{
		
		FVector LockLocation = GetTargetLockWidgetPosition(CachedTargetLockInterface.Get());

		float FovX = CachedHeroController->PlayerCameraManager->GetFOVAngle();
		int32 SizeX = 0;
		int32 SizeY = 0;
		CachedHeroController->GetViewportSize(SizeX, SizeY);

		float AspectRatio = (float)SizeX / (float)SizeY;
		float FovY = FMath::RadiansToDegrees(
			2.0f * FMath::Atan(FMath::Tan(FMath::DegreesToRadians(FovX / 2.0f)) / AspectRatio));
		/*the target point's position on the screen,here the position is not accurate,but it is enough from my test*/
		float AdjustPitch = FMath::Lerp(0, -FovY / 2.f,
										CachedTargetLockInterface->GetTargetLockComponent()->LockOffset);
		/*clamp the pitch value to make sure the view is comfortable*/
		float PitchValue = FMath::Clamp(
			(LockLocation - CachedHeroController->PlayerCameraManager->GetCameraLocation()).Rotation().Pitch
			+ AdjustPitch,
			CachedTargetLockInterface->GetTargetLockComponent()->MinPitch,
			CachedTargetLockInterface->GetTargetLockComponent()->MaxPitch);
		FRotator TargetRotation(
			PitchValue,
			(LockLocation - CachedHeroCharacter->GetActorLocation()).Rotation().Yaw, 0);

		DrawTargetLockWidget(CachedHeroController.Get());
		SetTargetLockWidgetPosition(LockLocation, CachedHeroController.Get());

		/*TODO:optimize the rotation*/
		CachedHeroController->SetControlRotation(TargetRotation);
	}
	
}

void UTargetLockComponent::InitTargetLockMappingContext()
{
	check(CachedHeroController.IsValid());
	const ULocalPlayer* LocalPlayer = CachedHeroController->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	Subsystem->AddMappingContext(TargetLockMappingContext, 2);
}

void UTargetLockComponent::ResetTargetLockMappingContext()
{
	if (CachedHeroController.IsValid())
	{
		const ULocalPlayer* LocalPlayer = CachedHeroController->GetLocalPlayer();
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
		check(Subsystem);
		Subsystem->RemoveMappingContext(TargetLockMappingContext);
	}
}

bool UTargetLockComponent::TryLockOnTarget()
{
	GetAvailableActorsToLock();
	if (AvailableActorsToLock.IsEmpty())
	{
		return false;
	}
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroController = GetOwingController<AActionHeroController>();
	}
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = GetOwner<AActionHeroCharacter>();
	}
	TargetActorToLock = GetNearestTargetFromAvailableActors(AvailableActorsToLock, CachedHeroController.Get());
	if (!TargetActorToLock) return false;
	CachedTargetLockInterface = TScriptInterface<ITargetLockInterface>(TargetActorToLock);
	if (CachedHeroCharacter.IsValid())
	{
		/*sometimes we will change the camera length and offset*/
		/*Todo: change the target arm length and target offset*/
		// CachedHeroCharacter->GetSprintArmComponent()->TargetArmLength += CachedTargetLockInterface->
		// 															 GetTargetLockComponent()->CameraLength;
		// CachedHeroCharacter->GetSprintArmComponent()->TargetOffset.Z += CachedTargetLockInterface->
		// 																GetTargetLockComponent()->CameraOffset;
		UpdateCameraAndTargetLock();
		CachedHeroCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = true;
		CachedHeroCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		SetComponentTickEnabled(true);
		InitTargetLockMappingContext();
		return true;
	}
	return false;
}

void UTargetLockComponent::GetAvailableActorsToLock()
{
	TArray<FHitResult> BoxTraceHits;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		GetOwner(),
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() *
		BoxTraceDistance,
		TraceBoxSize / 2.f,
		GetOwner()->GetActorForwardVector().ToOrientationRotator(),
		BoxTracChannel,
		false,
		IgnoreActors,
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
	);
	AvailableActorsToLock.Empty();
	for (const FHitResult& BoxTraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = BoxTraceHit.GetActor())
		{
			if (!UActionFunctionLibrary::NativeDoesActorHaveTag(HitActor, ActionGameplayTags::Shared_Status_Death))
				if (ITargetLockInterface* Interface = Cast<ITargetLockInterface>(HitActor))
				{
					{
						AvailableActorsToLock.AddUnique(HitActor);
					}
				}
		}
	}
}

void UTargetLockComponent::GetAvailableActorsAroundTarget(AActor* & OutActorOnLeft,
                                                          AActor*& OutActorOnRight)
{
	if (!TargetActorToLock)
	{
		return;
	}
	int32 SizeX = 0;
	int32 SizeY = 0;

	checkf(CachedHeroController.IsValid(), TEXT("HeroController is null"));
	CachedHeroController->GetViewportSize(SizeX, SizeY);
	TMap<AActor*, FVector2D> TargetScreenPositions;
	for (const auto Actor : AvailableActorsToLock)
	{
		FVector2D ScreenPosition;
		bool bOnScreen = CachedHeroController->ProjectWorldLocationToScreen(
			Actor->GetActorLocation(), ScreenPosition);
		if (bOnScreen && ScreenPosition.X >= 0 && ScreenPosition.X <= SizeX &&
			ScreenPosition.Y >= 0 && ScreenPosition.Y <= SizeY)
		{
			TargetScreenPositions.
				Add(TPair<AActor*, FVector2D>(Actor, FVector2D(ScreenPosition.X, ScreenPosition.Y)));
		}
	}
	if (TargetScreenPositions.Num() > 0)
	{
		FVector2D TargetActorPosition = FVector2D::ZeroVector;
		CachedHeroController->ProjectWorldLocationToScreen(
			TargetActorToLock->GetActorLocation(), TargetActorPosition);
		FVector2D ClosestPositionOnLeft = FVector2D(0, 0);
		FVector2D ClosestPositionOnRight = FVector2D(SizeX, SizeY);

		for (auto ScreenPosition : TargetScreenPositions)
		{
			if (ScreenPosition.Value.X < TargetActorPosition.X && ScreenPosition.Value.X >= ClosestPositionOnLeft.X)
			{
				ClosestPositionOnLeft = ScreenPosition.Value;
				OutActorOnLeft = ScreenPosition.Key;
			}
			if (ScreenPosition.Value.X > TargetActorPosition.X && ScreenPosition.Value.X <= ClosestPositionOnRight.X)
			{
				ClosestPositionOnRight = ScreenPosition.Value;
				OutActorOnRight = ScreenPosition.Key;
			}
		}
	}
}

AActor* UTargetLockComponent::GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActorsToLock,
                                                                  AActionHeroController* InPlayerController)
{
	AActor* TargetActor = nullptr;
	int32 SizeX = 0;
	int32 SizeY = 0;

	checkf(InPlayerController, TEXT("HeroController is null"));
	InPlayerController->GetViewportSize(SizeX, SizeY);
	TMap<AActor*, FVector2D> TargetScreenPositions;
	for (const auto Actor : AvailableActorsToLock)
	{
		FVector2D ScreenPosition;
		bool bOnScreen = InPlayerController->ProjectWorldLocationToScreen(
			Actor->GetActorLocation(), ScreenPosition);
		if (bOnScreen && ScreenPosition.X >= 0 && ScreenPosition.X <= SizeX &&
			ScreenPosition.Y >= 0 && ScreenPosition.Y <= SizeY)
		{
			TargetScreenPositions.
				Add(TPair<AActor*, FVector2D>(Actor, FVector2D(ScreenPosition.X, ScreenPosition.Y)));
		}
	}
	if (TargetScreenPositions.Num() > 0)
	{
		FVector2D CenterPosition(SizeX / 2.f, SizeY / 2.f);
		FVector2D ClosestPosition(SizeX, SizeY);

		for (auto ScreenPosition : TargetScreenPositions)
		{
			if (FVector2D::Distance(ScreenPosition.Value, CenterPosition) < FVector2D::Distance(
				ClosestPosition, CenterPosition))
			{
				ClosestPosition = ScreenPosition.Value;
				TargetActor = ScreenPosition.Key;
			}
		}
	}
	return TargetActor;
}

void UTargetLockComponent::DrawTargetLockWidget(AActionHeroController* InPlayerController)
{
	if (!DrawnTargetLockWidget)
	{
		checkf(TargetLockWidgetClass,
		       TEXT("TargetWidgetClass is not set!"));
		checkf(InPlayerController, TEXT("HeroController is null"));
		DrawnTargetLockWidget = CreateWidget<UActionWidgetBase>(InPlayerController, TargetLockWidgetClass);
	}
	check(DrawnTargetLockWidget);
	if (!DrawnTargetLockWidget->IsInViewport())
	{
		DrawnTargetLockWidget->AddToViewport();
	}
}

FVector UTargetLockComponent::GetTargetLockWidgetPosition(const ITargetLockInterface* TargetLockInterface)
{
	FVector LockLocation = TargetActorToLock->GetActorLocation();
	const FName TargetSocketName = TargetLockInterface->GetTargetLockComponent()->SocketName;
	if (TargetSocketName.IsValid())
	{
		USkeletalMeshComponent* SkeletalMesh = TargetActorToLock->FindComponentByClass<
			USkeletalMeshComponent>();
		check(SkeletalMesh);
		LockLocation = SkeletalMesh->GetSocketLocation(TargetSocketName);
	}
	return LockLocation;
}

void UTargetLockComponent::SetTargetLockWidgetPosition(const FVector& TargetLocation,
                                                       AActionHeroController* InPlayerController)
{
	if (!DrawnTargetLockWidget || !TargetActorToLock)
	{
		return;
	}
	checkf(TargetLockWidgetClass,
	       TEXT("TargetWidgetClass is not set!"));
	FVector2D ScreenPosition = FVector2D::ZeroVector;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		InPlayerController,
		TargetLocation, ScreenPosition, true);

	if (TargetLockWidgetSize == FVector2D::ZeroVector)
	{
		DrawnTargetLockWidget->WidgetTree->ForEachWidget(
			[this](UWidget* Widget)
			{
				if (USizeBox* SizeBox = Cast<USizeBox>(Widget))
				{
					TargetLockWidgetSize.X = SizeBox->GetWidthOverride();
					TargetLockWidgetSize.Y = SizeBox->GetHeightOverride();
				}
			});
	}
	ScreenPosition -= TargetLockWidgetSize / 2.f;
	DrawnTargetLockWidget->SetPositionInViewport(ScreenPosition, false);
}

void UTargetLockComponent::CleanUp()
{
	AvailableActorsToLock.Empty();
	TargetActorToLock = nullptr;
	if (DrawnTargetLockWidget)
	{
		DrawnTargetLockWidget->RemoveFromParent();
	}
	DrawnTargetLockWidget = nullptr;
	TargetLockWidgetSize = FVector2D::ZeroVector;

	SetComponentTickEnabled(false);
	OnLockStatuesChanged.Broadcast(false);
	if (CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter->GetCharacterMovement()->bUseControllerDesiredRotation = false;
		CachedHeroCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	}
	ResetTargetLockMappingContext();
	CachedTargetLockInterface = nullptr;
	CachedHeroCharacter = nullptr;
	CachedHeroController = nullptr;
}

void UTargetLockComponent::SwitchTarget(float XDirection)
{
	if (!TargetActorToLock)
	{
		return;
	}
	GetAvailableActorsToLock();
	AActor* ActorOnLeft = nullptr;
	AActor* ActorOnRight = nullptr;
	GetAvailableActorsAroundTarget(ActorOnLeft, ActorOnRight);

	if (XDirection > 0 && ActorOnRight)
	{
		TargetActorToLock = ActorOnRight;
	}
	else if (XDirection < 0 && ActorOnLeft)
	{
		TargetActorToLock = ActorOnLeft;
	}
}
