// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Library/StructLibrary.h"
#include "AttackCollisionComponent.generated.h"

class UAttackCollisionComponent;
DECLARE_DELEGATE_TwoParams(FOnHitDelegate, const FHitResult&, const UAttackCollisionComponent*);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONGAME_API UAttackCollisionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UAttackCollisionComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnRegister() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;


	FOnHitDelegate OnHit;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AttackCollision")
	TArray<FActionAttackCollisionParameters>AttackCollisionParameters;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AttackCollision")
	FActionHitParameters HitParameters;
	
	UFUNCTION(BlueprintCallable,Category="Action|Combat")
	void SetAttackCollisionComponent(TArray<FActionAttackCollisionParameters> InAttackCollisionParameters, USceneComponent* InParentComponent);

	void DestroyShapeComponents();


private:


	UFUNCTION()
	void CreatAndAttachAttackCollisions( USceneComponent* InParentComponent);

	UFUNCTION()
	void AttachAttackCollision(UShapeComponent* InShapeComponent,USceneComponent* InParentComponent,FName InSocketName,FTransform CollisionTransform);
	

	UPROPERTY()
	TArray<UShapeComponent*>AttackCollisions;

	UPROPERTY()
	TArray<FTransform>PreviousCollisionBoxTransforms;
	
	UPROPERTY()
	TArray<FTransform>CurrentCollisionBoxTransforms;

	UPROPERTY()
	TArray<UPrimitiveComponent*>IgnoreComponents;

};
