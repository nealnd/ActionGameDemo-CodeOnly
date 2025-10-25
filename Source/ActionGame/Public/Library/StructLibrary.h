#pragma once


#include "CoreMinimal.h"
#include "EnumLibrary.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "StructLibrary.generated.h"

class UGameplayAbility;
class UActionEnemyGameplayAbility;
class UInputMappingContext;
class UActionHeroAnimInstance;
class UActionGameplayAbility;

USTRUCT(BlueprintType)
struct FLocomotionDirectionSettings
{
	
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float FMin=-22.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float FMax=22.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float FRMin=22.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float FRMax=67.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float RMin=67.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float RMax=112.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BRMin=112.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BRMax=157.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BMin=-157.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BMax=157.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BLMin=-157.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float BLMax=-112.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float LMin=-112.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float LMax=-67.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float FLMin=-67.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float FLMax=-22.5f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float DeadZone=10.f;
};

USTRUCT(BlueprintType)
struct FGaitSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxWalkSpeed=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxAcceleration=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BrakingDeceleration=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BrakingFrictionFactor=0.f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bUseSeparateBrakingFriction=false;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BrakingFriction=0.f;
	
};

USTRUCT(BlueprintType)
struct FDirectionalAnims
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* F=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* FR=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* R=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* BR=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* B=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* BL=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* L=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* FL=nullptr;
};

USTRUCT(BlueprintType)
struct FDirectionalMontages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* F=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* FR=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* R=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* BR=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* B=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* BL=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* L=nullptr;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* FL=nullptr;
};

USTRUCT(BlueprintType)
struct FFootFXFootDefinition
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName BoneName;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName NotifyTrack;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UAnimNotify>AnimNotify;
	
	
};

USTRUCT(BlueprintType)
struct FActionHeroAbilitySet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(Categories="InputTag"))
	FGameplayTag InputTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UActionGameplayAbility>AbilityToGrant;

	bool IsValid() const;

};


USTRUCT(BlueprintType)
struct FActionWeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UActionHeroAnimInstance>WeaponAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UActionHeroAnimInstance>WeaponBlockAnimLayerToLink;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FDirectionalMontages DodgeMontages;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* IdleBlockStartMontage=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* WalkBlockStartMontage=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* IdleBlockEndMontage=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* WalkBlockEndMontage=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName WeaponEquipSocket;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName WeaponUnequipSocket;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D>SoftWeaponIconTexture;
	
};

USTRUCT(BlueprintType)
struct FActionAttackCollisionShapeParameters
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	EAttackCollisionShapeType CollisionShape=EAttackCollisionShapeType::Capsule;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = ( EditCondition = "CollisionShape == EAttackCollisionShapeType::Box",EditConditionHides="true" ))
	FVector BoxExtent= FVector(50.f, 50.f, 50.f);
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta = (EditCondition = "CollisionShape == EAttackCollisionShapeType::Capsule",EditConditionHides="true" ))
	float CapsuleRadius=35.f;;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (EditCondition = "CollisionShape == EAttackCollisionShapeType::Capsule",EditConditionHides="true" ))
	float CapsuleHalfHeight= 50.f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, meta = (EditCondition = "CollisionShape == EAttackCollisionShapeType::Sphere",EditConditionHides="true" ))
	float SphereRadius=50.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FTransform CollisionTransform=FTransform::Identity;
	
};

USTRUCT(BlueprintType)
struct FActionAttackCollisionParameters
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName SocketName;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	FActionAttackCollisionShapeParameters CollisionShapeParameters;
	
};

USTRUCT(BlueprintType)
struct FActionHitParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SKillDamagePercentage=1.f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BlockSpiritCost=1.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float PerfectBlockSpiritGain=1.f;

	/*Deciding the type of hit reaction animation, like inplace, knockback, or knockup and so on*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 HitLevel=1;

	/*hit animation stop time*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HitStopTime=0.f;

	
};

USTRUCT(Blueprintable)
struct FActionAIWeightedSkillParameter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DefaultWeight = 0.0f;

	UPROPERTY()
	float CurrentWeight =0.f;

	/*when ability is on cooldown, should it have weight or not,if true, it would have zero weight*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bIsResetOnCooldown=true;

	/*cooldown start when the ability start executing*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(EditCondition="bIsResetOnCooldown==true",EditConditionHides="true"))
	float CooldownTime=0.f;

	bool IsValid() const;
	
};

USTRUCT(Blueprintable)
struct FActionAIWeightedSkillConfig
{
	GENERATED_BODY()

	/*not include*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float MinDistance=0.f;

	/*include*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	float MaxDistance=0.f;

	/*base on the owner's rotation(yaw,-180~180),if MinAngle>MaxAngle, it means angle>MinAngle&&angle<MaxAngle,else MinAngle<=angle<=MaxAngle*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(ClampMin=-180.f,ClampMax=180.f))
	float MinAngle=0.f;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(ClampMin=-180.f,ClampMax=180.f))
	float MaxAngle=0.f;

	/*The same skill should have the same cooldown*/
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TMap<TSubclassOf<UGameplayAbility>,FActionAIWeightedSkillParameter> SkillData;
	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FActionAIWeightedSkillConfigContainer
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TArray<FActionAIWeightedSkillConfig> SkillConfigs;
	
};

/*use ability instead of animation is for more flexible*/
USTRUCT(Blueprintable)
struct FActionHitReactionAbility
{
	GENERATED_BODY()

public:
	/*if others are not specified, then it will be set to default*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> Default;

	/*direction is based on the attack animation's direction,
	 *left to right means the attack animation is from left to right,like the sword's first normal attack*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> LeftToRight;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> RightToLeft;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> TopToBottom;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> BottomToTop;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> FrontToBack;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UActionGameplayAbility> BackToFront;

	TArray<TSubclassOf<UActionGameplayAbility>> GetAllValidAbilities() const;

};

USTRUCT(BlueprintType)
struct FActionHitReactionAbilityContainer
{
	
	GENERATED_BODY()
public:
	/*this direction is based on the characters' relative position,
 *if the attacker is in front of the defender, then the direction is front*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitReactionAbility Front;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitReactionAbility Back;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitReactionAbility Left;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitReactionAbility Right;

	/*when the defender is blocking*/
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitReactionAbility Block;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FActionHitReactionAbility BlockBreak;

	
	void GetAllValidAbilities(TArray<TSubclassOf<UActionGameplayAbility>>&Abilities) const;
	
};

USTRUCT(BlueprintType)
struct FActionHitResultInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|HitInfo")
	FHitResult HitResult;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|HitInfo")
	AActor* Attacker=nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|HitInfo")
	FActionHitParameters HitParameters;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|HitInfo")
	bool bIsBlockSuccessfully=false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|HitInfo")
	bool bIsPerfectBlockSuccessfully=false;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Action|HitInfo")
	bool bIsBlockBreak=false;
};

USTRUCT(BlueprintType)
struct FActionGameLevelSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,meta=(Categories="GameData.Level"))
	FGameplayTag LevelTag;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld>Level;

	bool IsValid() const
	{
		return LevelTag.IsValid()&&!Level.IsNull();
	}
	
	
};
