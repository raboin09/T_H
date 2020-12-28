// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Animation/AnimSequence.h"
#include "Curves/CurveVector.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LocomotionAnimTypes.generated.h"

class ABaseCharacter;
class UCapsuleComponent;

UENUM(BlueprintType)
enum class EMovementDirection : uint8 {
	FORWARD UMETA(DisplayName = "Forward"),
	RIGHT UMETA(DisplayName = "Right"),
	LEFT UMETA(DisplayName = "Left"),
	BACKWARD UMETA(DisplayName = "Backward")
};

UENUM(BlueprintType)
enum class EAnimStance : uint8 {
	STANDING UMETA(DisplayName = "Standing"),
	CROUCHING UMETA(DisplayName = "Crouching")
};

UENUM(BlueprintType)
enum class EGroundedState : uint8 {
	NONE UMETA(DisplayName = "None"),
	ROLL UMETA(DisplayName = "Roll")
};

UENUM(BlueprintType)
enum class EHipsDirection : uint8 {
	F UMETA(DisplayName = "F"),
	B UMETA(DisplayName = "B"),
	RF UMETA(DisplayName = "RF"),
	RB UMETA(DisplayName = "RB"),
	LF UMETA(DisplayName = "LF"),
	LB UMETA(DisplayName = "LB")
};

UENUM(BlueprintType)
enum class EOverlayState : uint8 {
	DEFAULT UMETA(DisplayName = "Default"),
	MASCULINE UMETA(DisplayName = "Masculine"),
	FEMININE UMETA(DisplayName = "Feminine"),
	TORCH UMETA(DisplayName = "Torch"),
	PISTOL_1H UMETA(DisplayName = "Pistol 1H"),
	PISTOL_2H UMETA(DisplayName = "Pistol 2H"),
	BOW UMETA(DisplayName = "Bow"),
	BOX UMETA(DisplayName = "Box"),
	BARREL UMETA(DisplayName = "Barrel"),
	RIFLE UMETA(DisplayName = "Rifle"),
	INJURED UMETA(DisplayName = "Injured")
};

UENUM(BlueprintType)
enum class EViewMode : uint8 {
	FIRST_PERSON UMETA(DisplayName = "First Person"),
	THIRD_PERSON UMETA(DisplayName = "Third Person")
};

UENUM(BlueprintType)
enum class EStance : uint8 {
	STANDING UMETA(DisplayName = "Standing"),
	CROUCHING UMETA(DisplayName = "Crouching")
};

UENUM(BlueprintType)
enum class EGait : uint8 {
	WALKING UMETA(DisplayName = "Walking"),
	RUNNING UMETA(DisplayName = "Running"),
	SPRINTING UMETA(DisplayName = "Sprinting")
};

UENUM(BlueprintType)
enum class ERotationMode : uint8 {
	VELOCITY_DIRECTION UMETA(DisplayName = "Velocity Direction"),
	LOOKING_DIRECTION UMETA(DisplayName = "Looking Direction"),
	AIMING UMETA(DisplayName = "Aiming")
};

UENUM(BlueprintType)
enum class EMovementState : uint8 {
	NONE UMETA(DisplayName = "None"),
	GROUNDED UMETA(DisplayName = "Grounded"),
	IN_AIR UMETA(DisplayName = "In Air"),
	MANTLING UMETA(DisplayName = "Mantling"),
	RAGDOLL UMETA(DisplayName = "Ragdoll")
};

UENUM(BlueprintType)
enum class EMovementAction : uint8 {
	NONE UMETA(DisplayName = "None"),
	LOW_MANTLE UMETA(DisplayName = "Low Mantle"),
	HIGH_MANTLE UMETA(DisplayName = "High Mantle"),
	ROLLING UMETA(DisplayName = "Rolling"),
	GETTING_UP UMETA(DisplayName = "Getting Up")
};

UENUM(BlueprintType)
enum class EMantleType : uint8 {
	LOW_MANTLE UMETA(DisplayName = "Low Mantle"),
	HIGH_MANTLE UMETA(DisplayName = "High Mantle"),
	FALLING_CATCH UMETA(DisplayName = "Falling Catch")
};

USTRUCT(BlueprintType)
struct FTurnInPlaceAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
		UAnimSequenceBase* Animation;
	UPROPERTY(EditDefaultsOnly)
		float AnimatedAngle;
	UPROPERTY(EditDefaultsOnly)
		float PlayRate;
	UPROPERTY(EditDefaultsOnly)
		FName SlotName;
	UPROPERTY(EditDefaultsOnly)
		bool bScaleTurnAngle;
};

USTRUCT(BlueprintType)
struct FDynamicMontageParams
{
	GENERATED_BODY()
public:

	UAnimSequenceBase* Animation;

	float BlendInTime;
	float BlendOutTime;
	float PlayRate;
	float StartTime;

};

USTRUCT(BlueprintType)
struct FVelocityBlendStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		float Front;
	UPROPERTY(BlueprintReadOnly)
		float Back;
	UPROPERTY(BlueprintReadOnly)
		float Left;
	UPROPERTY(BlueprintReadOnly)
		float Right;

};

USTRUCT(BlueprintType)
struct FEssentialStatesStruct
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
		EMovementState MovementState;
	UPROPERTY(BlueprintReadOnly)
		EMovementState PreviousMovementState;
	UPROPERTY(BlueprintReadOnly)
		EMovementAction MovementAction;
	UPROPERTY(BlueprintReadOnly)
		ERotationMode RotationMode;
	UPROPERTY(BlueprintReadOnly)
		EGait Gait;
	UPROPERTY(BlueprintReadOnly)
		EStance Stance;
	UPROPERTY(BlueprintReadOnly)
		EViewMode ViewMode;
	UPROPERTY(BlueprintReadOnly)
		EOverlayState OverlayState;
};

USTRUCT(BlueprintType)
struct FEssentialValuesStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		FVector Velocity;
	UPROPERTY(BlueprintReadOnly)
		FVector Acceleration;
	UPROPERTY(BlueprintReadOnly)
		FVector MovementInput;
	
	UPROPERTY(BlueprintReadOnly)
		bool bIsMoving;
	UPROPERTY(BlueprintReadOnly)
		bool bHasMovementInput;
	
	UPROPERTY(BlueprintReadOnly)
		float Speed;
	UPROPERTY(BlueprintReadOnly)
		float MovementInputAmount;
	UPROPERTY(BlueprintReadOnly)
		float AimYawRate;
	
	UPROPERTY(BlueprintReadOnly)
		FRotator AimingRotation;
	UPROPERTY(BlueprintReadOnly)
		FRotator LastVelocityRotation;
	UPROPERTY(BlueprintReadOnly)
		FRotator LastMovementInputRotation;

};

USTRUCT(BlueprintType)
struct FAnimEssentialValuesStruct
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly)
		FEssentialStatesStruct MovementStates;
	UPROPERTY(BlueprintReadOnly)
		FEssentialValuesStruct CharValues;

};

USTRUCT(BlueprintType)
struct FCharInputValues
{
	GENERATED_BODY()

public:
	
	ERotationMode DesiredRotationMode;
	EGait DesiredGait;	
	EStance DesiredStance;
	
	float LookUpRate;
	float TurnAtRate;
	
	int32 TimesPressedStance;
	
	bool bBreakFall;
	bool bSprintHeld;

};

USTRUCT(BlueprintType)
struct FMantleParams
{
	GENERATED_BODY()

public:	

	UAnimMontage* AnimMontage;	
	UCurveVector* CorrectionCurve;

	float StartingPosition;	
	float PlayRate;	

	FVector StartingOffset;

};

USTRUCT(BlueprintType)
struct FLeanAmountStruct
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		float LeftRight;
	UPROPERTY(BlueprintReadOnly)
		float ForwardBack;
};

USTRUCT(BlueprintType)
struct FComponentAndTransformStruct
{
	GENERATED_BODY()

public:
	
	FTransform Transform;
	
	UPrimitiveComponent* Component;

};

USTRUCT(BlueprintType)
struct FMantleTraceSettings
{
	GENERATED_BODY()

public:
	
	float MaxLedgeHeight;	
	float MinLedgeHeight;	
	float ReachDistance;	
	float ForwardTraceRadius;
	float DownwardTraceRadius;

};

USTRUCT(BlueprintType)
struct FCameraSystemStruct
{
	GENERATED_BODY()

public:
	
	bool bRightShoulder;
	
	float ThirdPersonFOV;	
	float FirstPersonFOV;

};

USTRUCT(BlueprintType)
struct FRotationSystemStruct
{
	GENERATED_BODY()

public:
	
	FRotator TargetRotation;	
	FRotator InAirRotation;

	float YawOffset;

};

USTRUCT(BlueprintType)
struct FCharMovementSettingsStruct
{
	GENERATED_BODY()

public:
	
	float WalkSpeed;	
	float RunSpeed;	
	float SprintSpeed;
	
	UCurveVector* MovementCurve;	
	UCurveFloat* RotationRateCurve;

};

USTRUCT(BlueprintType)
struct FMantleSystemStruct
{
	GENERATED_BODY()

public:

	
	FMantleParams MantleParams;	
	FComponentAndTransformStruct MantleLedgeLS;

	FMantleTraceSettings GroundedTraceSettings;
	FMantleTraceSettings AutomaticeTraceSettings;
	FMantleTraceSettings FallingTraceSettings;
	
	FTransform MantleTarget;
	FTransform MantleActualOffset;	
	FTransform MantleAnimatedStartOffset;

};

USTRUCT(BlueprintType)
struct FMovementSettingsStance
{
	GENERATED_BODY()

public:

	FCharMovementSettingsStruct Standing;
	FCharMovementSettingsStruct Crouching;

};

USTRUCT(BlueprintType)
struct FMovementSettingsState
{
	GENERATED_BODY()

public:
	
	FMovementSettingsStance VelocityDirection;	
	FMovementSettingsStance LookingDirection;	
	FMovementSettingsStance Aiming;

};

USTRUCT(BlueprintType)
struct FCharMovementValues
{
	GENERATED_BODY()

public:
	void Tick(float InDeltaTimeX);

	void SetPlayerCharacter(ABaseCharacter* InChar);
	void CacheValues();

	FEssentialValuesStruct GetEssentialValues();
	void AssignEssentialValues();

	EGait GetAllowedGait();
	EGait GetActualGait();
	void SetGait(EGait InGait);

	void UpdateCharacterMovement();
	void UpdateDynamicMovementSettings(EGait InAllowedGait);
	void UpdateGroundedRotation();
	void UpdateInAirRotation();
	void UpdateRagdoll();
	void SetActorLocationDuringRagdoll();
	FHitResult SetActorLocationAndRotation_UpdateTarget(FVector InLoc, FRotator InRot, bool bSweep, bool bTeleport);

	void SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed);
	void LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed);
	void MantleStart(float MantleHeight, FTransform MantleLedgeWSTransform, UPrimitiveComponent* MantleLedgeWSComponent, EMantleType MantleType);

	FCharMovementSettingsStruct GetTargetMovementSettings();
	float GetMappedSpeed();
	float GetAnimCurveValue(FName CurveName);
	FVector GetCapsuleBaseLocation(float ZOffset);
	FVector GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset);
	FVector GetPlayerMovementInput();
	float CalculateGroundedRotationRate();

	bool CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset);
	bool CanUpdateMovingRotation();
	bool CanSprint();
	bool MantleCheck(FMantleTraceSettings InTraceSettings);


public:
	FName ROOT_BONE = "root";
	FName PELVIS_BONE = "pelvis";
	FName ROTATION_AMOUNT = "RotationAmount";
	FName YAW_OFFSET = "YawOffset";
	FName MOVE_FORWARD_AXIS = "MoveForward";
	FName MOVE_RIGHT_AXIS = "MoveRight";
	FName PAWN_CHANNEL = "Pawn";
	
	ABaseCharacter* PlayerCharacter;	
	FAnimEssentialValuesStruct EssentialValues;

	// INPUT VALUES
	FCharInputValues InputValues;

	// CACHED VALUES
	FVector PreviousVelocity;	
	float PreviousAimYaw;

	// GAITS	
	EGait AllowedGait;	
	EGait ActualGait;	
	EGait PreviousActualGait;

	// RAGDOLL	
	bool bRagdollOnGround;	
	bool bRagdollFaceUp;

	FVector LastRagdollVelocity;

	// MANTLE SYSTEM	
	FMantleSystemStruct MantleSystem;	
	EMantleType MantleType;

	// ROTATION SYSTEM	
	FRotationSystemStruct RotationSystem;

	// CAMERA SYSTEM	
	FCameraSystemStruct CameraSystem;

	// MOVEMENT SYSTEM	
	FCharMovementSettingsStruct CurrentMovementSettings;	
	FMovementSettingsState MovementData;

};