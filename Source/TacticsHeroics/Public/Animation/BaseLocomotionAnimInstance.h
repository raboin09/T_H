// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Actors/BaseCharacter.h"
#include "Types/LocomotionAnimTypes.h"
#include "BaseLocomotionAnimInstance.generated.h"

UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class TACTICSHEROICS_API UBaseLocomotionAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTimeX) override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
		float GetDistanceBetweenTwoSockets(USkeletalMeshComponent* Component, FName SocketA, ERelativeTransformSpace SocketSpaceA, FName SocketB, ERelativeTransformSpace SocketSpaceB, bool bRemapRange, float InRangeMin, float InRangeMax, float OutRangeMin, float OutRangeMax);

	virtual void UpdatePawnOwner();

private:

	virtual void PlayTransition(FDynamicMontageParams Parameters);
	virtual void PlayDynamicTransition(float RetriggerDelay, FDynamicMontageParams Parameters);

	//////////////////////////
	//    *** EVENTS     ***//
	//////////////////////////

	virtual void Jumped();
	virtual void SetGroundedEntryState(EGroundedState InState);
	virtual void SetOverlayOverrideState(int32 InState);

	//////////////////////////
	// *** ANIM NOTIFIES ***//
	//////////////////////////

	virtual void AnimNotify_StopTransition();

	virtual void AnimNotify_N_Stop_R();
	virtual void AnimNotify_N_Stop_L();
	virtual void AnimNotify_CLF_Stop();
	virtual void AnimNotify_N_QuickStop();

	virtual void AnimNotify_RollToIdle();
	virtual void AnimNotify_LandToIdle();

	virtual void AnimNotify_BowMakeReady();
	virtual void AnimNotify_BowMakeRelaxed();

	virtual void AnimNotify_Pivot();

	virtual void AnimNotify_Hips_F();
	virtual void AnimNotify_Hips_B();
	virtual void AnimNotify_Hips_LF();
	virtual void AnimNotify_Hips_LB();
	virtual void AnimNotify_Hips_RF();
	virtual void AnimNotify_Hips_RB();

	virtual void AnimNotify_ResetGroundedEntryState();

	/////////////////////////////
	// *** TICK FUNCTIONS ***  //
	/////////////////////////////

	virtual void UpdateAimingValues();
	virtual void UpdateLayerValues();
	virtual void UpdateFootIK();
	virtual void UpdateInAirValues();
	virtual void UpdateRagdollValues();
	virtual void UpdateMovementValues();
	virtual void UpdateRotationValues();

	/////////////////////////////
	// *** HELPER FUNCTIONS ***//
	/////////////////////////////

	void SetFootLocking(FName EnableFootIDCurve, FName FootLockCurve, FName IKFootBone, float CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation);
	void SetFootLockOffsets(FRotator& InRotation, FVector& InLocation);
	virtual bool CanOverlayTransition();
	FDynamicMontageParams CreateTransitionMontageStruct();
	float CalculateLandPrediction();
	FLeanAmountStruct InterpLeanAmount(FLeanAmountStruct Current, FLeanAmountStruct Target, float InterpSpeed, float DeltaTime);
	FLeanAmountStruct CalculateInAirLeanAmount();
	float CalculateDiagonalScaleAmount();
	float CalculateWalkRunBlend();
	float CalculateStrideBlend();
	float CalculateStandingPlayRate();
	float CalculateCrouchingPlayRate();
	EMovementDirection CalculateMovementDirection();
	void TurnInPlace(FRotator InTargetRotation, float PlayRateScale, float StartTime, bool bOverrideCurrent);

	FVelocityBlendStruct InterpVelocityBlend(FVelocityBlendStruct Current, FVelocityBlendStruct Target, float InterpSpeed, float DeltaTime);
	FVelocityBlendStruct CalculateVelocityBlend();
	FVector CalculateRelativeAccelerationAmount();
	EMovementDirection CalculateQuadrant(EMovementDirection Current, float FR_Threshold, float FL_Threshold, float BR_Threshold, float BL_Threshold, float Buffer, float Angle);
	bool IsAngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool bIncreaseBuffer);

	bool ShouldMoveCheck();

	bool CanRotateInPlace();
	bool CanTurnInPlace();
	bool CanDynamicTransition();

	void RotateInPlaceCheck();
	void TurnInPlaceCheck();
	void DynamicTransitionCheck();

	float GetAnimCurveClamp(FName Name, float Bias = -1.0, float ClampMin = 0.0, float ClampMax = 1.0);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		ABaseCharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimSequenceBase* TransitionSequence;

	const FName GROUNDED_SLOT = "Grounded Slot";
	const FName N_TURN_ROTATE_SLOT = "(N) Turn/Rotate";
	const FName CLF_TURN_ROTATE = "(CLF)Turn / Rotate";

	const FName ENABLE_FOOTIK_L = "Enable_FootIK_L";
	const FName FOOTLOCK_L = "FootLock_L";
	const FName ENABLE_FOOTIK_R = "Enable_FootIK_R";
	const FName FOOTLOCK_R = "FootLock_R";

	const FName IK_FOOT_L = "ik_foot_l";
	const FName IK_FOOT_R = "ik_foot_R";
	const FName VB_FOOT_TARGET_L = "VB foot_target_l";
	const FName VB_FOOT_TARGET_R = "VB foot_target_r";

	const FName WEIGHT_GAIT = "Weight_Gait";
	const FName BASE_POSE_CLF = "BasePose_CLF";

	const FName ENABLE_TRANSITION = "Enable_Transition";

	const FName ROOT = "root";

	const FName MASK_LAND_PREDICTION = "Mask_LandPrediction";

	float SmoothedAimingRotationInterpSpeed;
	
	UPROPERTY(BlueprintReadOnly)
		EAnimStance Stance;
	UPROPERTY(BlueprintReadOnly)
		EGroundedState GroundedEntryState;
	UPROPERTY(BlueprintReadOnly)
		EHipsDirection TrackedHipsDirection;

	// GROUNDED
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		FLeanAmountStruct LeanAmount;

	UPROPERTY(BlueprintReadOnly)
		FVelocityBlendStruct VelocityBlend;
	UPROPERTY(BlueprintReadOnly)
		FVector RelativeAccelerationAmount;
	UPROPERTY(BlueprintReadOnly)
		EMovementDirection MovementDirection;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float DiagonalScaleAmount;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float WalkRunBlend;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float StandingPlayRate;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float StrideBlend;

	UPROPERTY(BlueprintReadOnly)
		float CrouchingPlayRate;

	UPROPERTY(BlueprintReadOnly)
		float RotateRate;
	UPROPERTY(BlueprintReadOnly)
		float RotationScale;

	UPROPERTY(BlueprintReadOnly)
		float FYaw;
	UPROPERTY(BlueprintReadOnly)
		float BYaw;
	UPROPERTY(BlueprintReadOnly)
		float LYaw;
	UPROPERTY(BlueprintReadOnly)
		float RYaw;

	UPROPERTY(BlueprintReadOnly)
		bool bRotate_L;
	UPROPERTY(BlueprintReadOnly)
		bool bRotate_R;

	// TURN IN PLACE
	UPROPERTY(EditDefaultsOnly)
		float ElapsedDelayTime;
	UPROPERTY(EditDefaultsOnly)
		float TurnCheckMinAngle;
	UPROPERTY(EditDefaultsOnly)
		float AimYawRateLimit;
	UPROPERTY(EditDefaultsOnly)
		float MinAngleDelay;
	UPROPERTY(EditDefaultsOnly)
		float MaxAngleDelay;
	UPROPERTY(EditDefaultsOnly)
		float Turn180Threshold;

	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset N_TurnIP_L90;
	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset N_TurnIP_R90;
	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset N_TurnIP_L180;
	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset N_TurnIP_R180;

	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset CLF_TurnIP_L90;
	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset CLF_TurnIP_R90;
	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset CLF_TurnIP_L180;
	UPROPERTY(EditDefaultsOnly)
		FTurnInPlaceAsset CLF_TurnIP_R180;

	// RAGDOLL 
	UPROPERTY(BlueprintReadOnly)
		float FlailRate;

	// IN AIR
	UPROPERTY(BlueprintReadOnly)
		bool bJumped;
	UPROPERTY(BlueprintReadOnly)
		float JumpPlayRate;
	UPROPERTY(BlueprintReadOnly)
		float FallSpeed;
	UPROPERTY(BlueprintReadOnly)
		float LandPrediction;
	UPROPERTY(BlueprintReadOnly)
		float InAirLeanInterpAmount;

	// BLEND CURVES
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* LandPredictionCurve;
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* LeanInAirCurve;
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* DiagonalScaleAmountCurve;
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* StrideBlend_N_Walk;
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* StrideBlend_N_Run;
	UPROPERTY(EditDefaultsOnly)
		UCurveFloat* StrideBlend_C_Walk;
	UPROPERTY(EditDefaultsOnly)
		UCurveVector* YawOffset_FB;
	UPROPERTY(EditDefaultsOnly)
		UCurveVector* YawOffset_LR;

	// ROTATE IN PLACE
	UPROPERTY(BlueprintReadOnly)
		float RotateMinThreshold;
	UPROPERTY(BlueprintReadOnly)
		float RotateMaxThreshold;

	UPROPERTY(BlueprintReadOnly)
		float AimYawRateMinRange;
	UPROPERTY(BlueprintReadOnly)
		float AimYawRateMaxRange;

	UPROPERTY(BlueprintReadOnly)
		float MaxPlayRate;
	UPROPERTY(BlueprintReadOnly)
		float MinPlayRate;

	// AIMING
	UPROPERTY(BlueprintReadOnly)
		FVector2D AimingAngle;
	UPROPERTY(BlueprintReadOnly)
		FVector2D SmoothedAimingAngle;
	UPROPERTY(BlueprintReadOnly)
		FRotator SpineRotation;
	UPROPERTY(BlueprintReadOnly)
		float AimSweepTime;
	UPROPERTY(BlueprintReadOnly)
		float InputYawOffsetTime;
	UPROPERTY(BlueprintReadOnly)
		float InputYawOffsetInterpTime;
	UPROPERTY(BlueprintReadOnly)
		float LeftYawTime;
	UPROPERTY(BlueprintReadOnly)
		float RightYawTime;
	UPROPERTY(BlueprintReadOnly)
		float ForwardYawTime;

	// ANIM GRAPH - LAYERING
	UPROPERTY(BlueprintReadOnly)
		float Enable_AimOffset;

	UPROPERTY(BlueprintReadOnly)
		float BasePose_N;
	UPROPERTY(BlueprintReadOnly)
		float BasePose_CLF;

	UPROPERTY(BlueprintReadOnly)
		float Arm_L;
	UPROPERTY(BlueprintReadOnly)
		float Arm_L_Add;
	UPROPERTY(BlueprintReadOnly)
		float Arm_L_LS;
	UPROPERTY(BlueprintReadOnly)
		float Arm_L_MS;

	UPROPERTY(BlueprintReadOnly)
		float Arm_R;
	UPROPERTY(BlueprintReadOnly)
		float Arm_R_Add;
	UPROPERTY(BlueprintReadOnly)
		float Arm_R_LS;
	UPROPERTY(BlueprintReadOnly)
		float Arm_R_MS;

	UPROPERTY(BlueprintReadOnly)
		float Hand_L;
	UPROPERTY(BlueprintReadOnly)
		float Hand_R;
	UPROPERTY(BlueprintReadOnly)
		float Legs;
	UPROPERTY(BlueprintReadOnly)
		float Legs_Add;

	UPROPERTY(BlueprintReadOnly)
		float Pelvis;
	UPROPERTY(BlueprintReadOnly)
		float Pelvis_Add;

	UPROPERTY(BlueprintReadOnly)
		float Spine;
	UPROPERTY(BlueprintReadOnly)
		float Spine_Add;

	UPROPERTY(BlueprintReadOnly)
		float Head;
	UPROPERTY(BlueprintReadOnly)
		float Head_Add;

	UPROPERTY(BlueprintReadOnly)
		float Enable_HandIK_L;
	UPROPERTY(BlueprintReadOnly)
		float Enable_HandIK_R;

	// FOOT LOCKING
	UPROPERTY(BlueprintReadOnly)
		float FootLock_L_Alpha;
	UPROPERTY(BlueprintReadOnly)
		FVector FootLock_L_Location;
	UPROPERTY(BlueprintReadOnly)
		FRotator FootLock_L_Rotation;
	UPROPERTY(BlueprintReadOnly)
		FVector FootOffset_L_Location;
	UPROPERTY(BlueprintReadOnly)
		FRotator FootOffset_L_Rotation;

	UPROPERTY(BlueprintReadOnly)
		float FootLock_R_Alpha;
	UPROPERTY(BlueprintReadOnly)
		FVector FootLock_R_Location;
	UPROPERTY(BlueprintReadOnly)
		FRotator FootLock_R_Rotation;
	UPROPERTY(BlueprintReadOnly)
		FVector FootOffset_R_Location;
	UPROPERTY(BlueprintReadOnly)
		FRotator FootOffset_R_Rotation;

	UPROPERTY(BlueprintReadOnly)
		float Pelvis_Alpha;
	UPROPERTY(BlueprintReadOnly)
		FVector PelvisOffset;

	// CONFIG
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float VelocityBlendInterpSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float GroundedLeanInterpAmount;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AnimatedWalkSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AnimatedRunSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AnimatedSprintSpeed;
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AnimatedCrouchSpeed;

	// BASE
	int32 OverlapOverrideState;

	UPROPERTY(BlueprintReadOnly)
		bool bShouldMove;
	bool bPrevShouldMove;
	bool bPivot;

	float TiggerPivotSpeedLimit;
	float DeltaTimeX;

	UPROPERTY(BlueprintReadOnly)
		FAnimEssentialValuesStruct CharValues;

private:

};
