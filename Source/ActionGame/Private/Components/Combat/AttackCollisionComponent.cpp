// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Combat/AttackCollisionComponent.h"



#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"


UAttackCollisionComponent::UAttackCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled=true;
	
}

void UAttackCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
	for (const auto&AttackCollision  : AttackCollisions)
	{
		PreviousCollisionBoxTransforms.Add( AttackCollision->GetComponentTransform());
		CurrentCollisionBoxTransforms.Add( AttackCollision->GetComponentTransform());
	}
}

void UAttackCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(GetOwner());
	for (int32 i = 0; i < AttackCollisions.Num(); i++)
	{
		UShapeComponent* CollisionBox = AttackCollisions[i];
		CurrentCollisionBoxTransforms[i] = CollisionBox->GetComponentTransform();
		
		FTransform PrevCollisionBoxTransform = PreviousCollisionBoxTransforms[i];
		FTransform CurCollisionBoxTransform = CurrentCollisionBoxTransforms[i];
		int32 Max = (CurCollisionBoxTransform.GetLocation() - PrevCollisionBoxTransform.GetLocation()).Length() / 40.f +
			1;
		for (int32 j = 0; j < Max; j++)
		{
			FTransform PreviousCollisionBoxTransformLerp;
			PreviousCollisionBoxTransformLerp.Blend(PrevCollisionBoxTransform, CurCollisionBoxTransform,
			                                        (float)j / (float)Max);

			FTransform CurrentCollisionBoxTransformLerp;
			CurrentCollisionBoxTransformLerp.Blend(PrevCollisionBoxTransform, CurCollisionBoxTransform,
			                                       (float)(j + 1) / (float)Max);

	
			TArray<FHitResult> OutHits;
			bool bHit = GetOwner()->GetWorld()->SweepMultiByChannel(
				OutHits, PreviousCollisionBoxTransformLerp.GetLocation(),
				CurrentCollisionBoxTransformLerp.GetLocation(),
				PreviousCollisionBoxTransformLerp.GetRotation(),
				ECC_GameTraceChannel1, CollisionBox->GetCollisionShape(),
				QueryParams,
				FCollisionResponseParams::DefaultResponseParam);
			
			if (!OutHits.IsEmpty())
			{
				for (auto& HitResult : OutHits)
				{
					if (!IgnoreComponents.Contains(HitResult.GetComponent()))
					{
						OnHit.ExecuteIfBound(HitResult,this);
						IgnoreComponents.AddUnique(HitResult.GetComponent());
					}
				
				}
			}
		}
		//DrawDebugCapsule(GetOwner()->GetWorld(),CurrentCollisionBoxTransforms[i].GetLocation(),60,20,CurrentCollisionBoxTransforms[i].GetRotation(),FColor::Blue,false,5.f,0,0.5f);
		PreviousCollisionBoxTransforms[i] = CurrentCollisionBoxTransforms[i];
	}
}

void UAttackCollisionComponent::OnRegister()
{
	Super::OnRegister();
	CreatAndAttachAttackCollisions(GetAttachParent());
}

void UAttackCollisionComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	Super::OnComponentDestroyed(bDestroyingHierarchy);
	DestroyShapeComponents();
}

void UAttackCollisionComponent::SetAttackCollisionComponent(
	TArray<FActionAttackCollisionParameters> InAttackCollisionParameters, USceneComponent* InParentComponent)
{
	AttackCollisionParameters = InAttackCollisionParameters;
	PreviousCollisionBoxTransforms.Empty();
	CurrentCollisionBoxTransforms.Empty();
	CreatAndAttachAttackCollisions(InParentComponent);
}

void UAttackCollisionComponent::DestroyShapeComponents()
{
	if (!AttackCollisions.IsEmpty())
	{
		for (UShapeComponent* AttackCollisionShapeComponent : AttackCollisions)
		{
			if (AttackCollisionShapeComponent)
			{
				AttackCollisionShapeComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
				AttackCollisionShapeComponent->DestroyComponent();
				AttackCollisionShapeComponent=nullptr;
			}
		}
	}
	AttackCollisions.Empty();
}

void UAttackCollisionComponent::CreatAndAttachAttackCollisions(USceneComponent* InParentComponent)
{
	DestroyShapeComponents();
	for (const FActionAttackCollisionParameters& CollsionParameter : AttackCollisionParameters)
	{
		switch (CollsionParameter.CollisionShapeParameters.CollisionShape)
		{
		case EAttackCollisionShapeType::Box:
			{
				UBoxComponent* AttackCollisionBox = NewObject<UBoxComponent>(GetOwner());
				AttackCollisionBox->SetBoxExtent(CollsionParameter.CollisionShapeParameters.BoxExtent);
				AttachAttackCollision(AttackCollisionBox, InParentComponent, CollsionParameter.SocketName,
				                      CollsionParameter.CollisionShapeParameters.CollisionTransform);
				break;
			}
		case EAttackCollisionShapeType::Capsule:
			{
				UCapsuleComponent* AttackCollisionCapsule = NewObject<UCapsuleComponent>(GetOwner());
				AttackCollisionCapsule->
					SetCapsuleSize(CollsionParameter.CollisionShapeParameters.CapsuleRadius,
					               CollsionParameter.CollisionShapeParameters.CapsuleHalfHeight);
				AttachAttackCollision(AttackCollisionCapsule, InParentComponent, CollsionParameter.SocketName,
				                      CollsionParameter.CollisionShapeParameters.CollisionTransform);
				break;
			}
		case EAttackCollisionShapeType::Sphere:
			{
				USphereComponent* AttackCollisionSphere = NewObject<USphereComponent>(GetOwner());
				AttackCollisionSphere->SetSphereRadius(CollsionParameter.CollisionShapeParameters.SphereRadius);
				AttachAttackCollision(AttackCollisionSphere, InParentComponent, CollsionParameter.SocketName,
				                      CollsionParameter.CollisionShapeParameters.CollisionTransform);
				break;
			}
		default:
			break;
		}
	
	}
}

void UAttackCollisionComponent::AttachAttackCollision(UShapeComponent* InShapeComponent,
                                                      USceneComponent* InParentComponent, FName InSocketName,
                                                      FTransform CollisionTransform)
{
	InShapeComponent->RegisterComponent();
	InShapeComponent->AttachToComponent(InParentComponent, FAttachmentTransformRules::SnapToTargetIncludingScale,
	                                    InSocketName);
	InShapeComponent->SetRelativeTransform(CollisionTransform);
	InShapeComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	AttackCollisions.Add(InShapeComponent);
}
