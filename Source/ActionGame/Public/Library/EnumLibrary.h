#pragma once

UENUM(BlueprintType)
enum class ELocomotionDirections : uint8
{
	F,
	FR,
	R,
	BR,
	B,
	BL,
	L,
	FL
};

UENUM(BlueprintType)
enum class EGait: uint8
{
	Walk,
	Run,
};


UENUM(BlueprintType)
enum class EMovementDirection : uint8
{
	Forward,
	Right,
	Left,
	Backward
};

UENUM(BlueprintType)
enum class EHipFacing : uint8
{
	Forward,
	Backward
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	Accumulate,
	BlendOut,
	Hold
	
};

UENUM(BlueprintType)
enum class EActionAbilityActivationPolicy : uint8
{
	OnTriggered,
	OnGiven
};

UENUM()
enum class EActionConfirmType:uint8
{
	Yes,
	No
};

UENUM()
enum class EActionValidType:uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EActionSuccessType:uint8
{
	Successful,
	Failed
};

UENUM(BlueprintType)
enum class EAttackCollisionShapeType : uint8
{
	Capsule,
	Box,
	Sphere
};

UENUM(BlueprintType)
enum class EActionGameModeState: uint8
{
	Begin,
	PlayerDied,
	Success
};

UENUM(BlueprintType)
enum class EActionInputMode: uint8
{
	GameOnly,
	UIOnly
};
