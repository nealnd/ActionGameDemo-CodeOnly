// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/PawnExtensionComponentBase.h"
#include "TargetLockComponent.generated.h"


class UInputMappingContext;
class ITargetLockInterface;
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLockStatuesChanged, bool bCanContinueLocking);

class AActionHeroCharacter;
class AActionHeroController;
class UActionWidgetBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UTargetLockComponent : public UPawnExtensionComponentBase
{
	GENERATED_BODY()

public:
	UTargetLockComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	FOnLockStatuesChanged OnLockStatuesChanged;

	bool TryLockOnTarget();

	void CleanUp();

	void SwitchTarget(float XDirection);

protected:
	/*the bone name that want to lock on*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|BeLocked")
	FName SocketName = NAME_None;

	/*the offset based on the screen, 0 means the center of the screen, 1 means the edge on top center of the screen*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|BeLocked")
	float LockOffset = 0.f;

	/*the max pitch when lock on,means the max angle we toward upside*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|BeLocked")
	float MaxPitch = 60.f;

	/*the min pitch when lock on,means the max angle we toward downside*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|BeLocked")
	float MinPitch = -30.f;

	/*sometimes we want to change the camera length when lock on,would add to the spring arm length*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|BeLocked")
	float CameraLength = 0.f;

	/*this would add to the target offset in spring arm*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|BeLocked")
	float CameraOffset = 0.f;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Parameters|Lock")
	TSubclassOf<UActionWidgetBase> TargetLockWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category="Parameters|Lock")
	float BoxTraceDistance = 5000.f;

	UPROPERTY(EditDefaultsOnly, Category="Parameters|Lock")
	FVector TraceBoxSize = FVector(5000.f, 5000.f, 300.f);

	UPROPERTY(EditDefaultsOnly, Category="Parameters|Lock")
	TArray<TEnumAsByte<EObjectTypeQuery>> BoxTracChannel;

	UPROPERTY(EditDefaultsOnly, Category="Parameters|Lock")
	bool bShowPersistentDebugShape = false;

	UPROPERTY(EditDefaultsOnly, Category="Parameters|Lock")
	UInputMappingContext* TargetLockMappingContext = nullptr;

private:
	UPROPERTY()
	TArray<AActor*> AvailableActorsToLock;

	UPROPERTY()
	AActor* TargetActorToLock = nullptr;

	UPROPERTY()
	UActionWidgetBase* DrawnTargetLockWidget = nullptr;

	UPROPERTY()
	FVector2D TargetLockWidgetSize = FVector2D::ZeroVector;

	TWeakInterfacePtr<ITargetLockInterface> CachedTargetLockInterface;

	UPROPERTY()
	TWeakObjectPtr<AActionHeroCharacter> CachedHeroCharacter;

	UPROPERTY()
	TWeakObjectPtr<AActionHeroController> CachedHeroController;

	void GetAvailableActorsToLock();
	void GetAvailableActorsAroundTarget(AActor*& OutActorOnLeft,
	                                    AActor*& OutActorOnRight);
	AActor* GetNearestTargetFromAvailableActors(const TArray<AActor*>& InAvailableActorsToLock,
	                                            AActionHeroController* InPlayerController);
	void DrawTargetLockWidget(AActionHeroController* InPlayerController);

	FVector GetTargetLockWidgetPosition(const ITargetLockInterface* TargetLockInterface);
	void SetTargetLockWidgetPosition(const FVector& TargetLocation, AActionHeroController* InPlayerController);
	void UpdateCameraAndTargetLock();
	void InitTargetLockMappingContext();
	void ResetTargetLockMappingContext();
	
};
