#include "Animation/BaseLocomotionAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Curves/CurveFloat.h"
#include "Actors/BaseCharacter.h"

void UBaseLocomotionAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	UpdatePawnOwner();
}

void UBaseLocomotionAnimInstance::NativeUpdateAnimation(float InDeltaTimeX)
{
	Super::NativeUpdateAnimation(DeltaTimeX);
	if (!PlayerCharacter) {
		UpdatePawnOwner();
	}
	DeltaTimeX = InDeltaTimeX;
	if (DeltaTimeX != 0.f) {
		if (PlayerCharacter) {
			UpdateAimingValues();
			UpdateLayerValues();
			UpdateFootIK();

			switch (CharValues.MovementStates.MovementState)
			{
			default:
				break;
			case EMovementState::GROUNDED:
				bPrevShouldMove = bShouldMove;
				bShouldMove = ShouldMoveCheck();
				if (bPrevShouldMove == true && bShouldMove == true) {
					UpdateMovementValues();
					UpdateRotationValues();
				}
				else if (bPrevShouldMove == false && bShouldMove == false) {
					if (CanRotateInPlace()) {
						RotateInPlaceCheck();
					}
					else {
						bRotate_L = false;
						bRotate_R = false;
					}
					if (CanTurnInPlace()) {
						TurnInPlaceCheck();
					}
					else {
						ElapsedDelayTime = 0.f;
					}
					if (CanDynamicTransition()) {
						DynamicTransitionCheck();
					}
				}
				else if (bPrevShouldMove == false && bShouldMove == true) {
					ElapsedDelayTime = 0.f;
					bRotate_L = false;
					bRotate_R = false;
				}
				else {

				}
				break;
			case EMovementState::IN_AIR:
				UpdateInAirValues();
				break;
			case EMovementState::RAGDOLL:
				UpdateRagdollValues();
				break;
			}
		}
	}
}

void UBaseLocomotionAnimInstance::UpdatePawnOwner()
{
	PlayerCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
}

void UBaseLocomotionAnimInstance::PlayTransition(FDynamicMontageParams Parameters)
{
	PlaySlotAnimationAsDynamicMontage(Parameters.Animation, GROUNDED_SLOT, Parameters.BlendInTime, Parameters.BlendOutTime, Parameters.PlayRate, 1, 0.0f, Parameters.StartTime);
}

void UBaseLocomotionAnimInstance::PlayDynamicTransition(float RetriggerDelay, FDynamicMontageParams Parameters)
{
	PlaySlotAnimationAsDynamicMontage(Parameters.Animation, GROUNDED_SLOT, Parameters.BlendInTime, Parameters.BlendOutTime, Parameters.PlayRate, 1, 0.0f, Parameters.StartTime);
	FTimerHandle RetriggerHandle;
	FTimerDelegate RetriggerDelegate = FTimerDelegate::CreateLambda([=]() {
		PlaySlotAnimationAsDynamicMontage(Parameters.Animation, GROUNDED_SLOT, Parameters.BlendInTime, Parameters.BlendOutTime, Parameters.PlayRate, 1, 0.0f, Parameters.StartTime);
	});
	GetWorld()->GetTimerManager().SetTimer(RetriggerHandle, RetriggerDelegate, RetriggerDelay, false);
}

void UBaseLocomotionAnimInstance::Jumped()
{
	bJumped = true;
	JumpPlayRate = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 600.f), FVector2D(1.2f, 1.5f), CharValues.CharValues.Speed);
	FTimerHandle JumpHandle;
	GetWorld()->GetTimerManager().SetTimer(JumpHandle, [this]() {
		bJumped = false;
	}, .1f, false);
}

void UBaseLocomotionAnimInstance::SetGroundedEntryState(EGroundedState InState)
{
	GroundedEntryState = InState;
}

void UBaseLocomotionAnimInstance::SetOverlayOverrideState(int32 InState)
{
	OverlapOverrideState = InState;
}

void UBaseLocomotionAnimInstance::AnimNotify_StopTransition()
{
	StopSlotAnimation(.2, GROUNDED_SLOT);
	StopSlotAnimation(.2, N_TURN_ROTATE_SLOT);
	StopSlotAnimation(.2, CLF_TURN_ROTATE);
}

void UBaseLocomotionAnimInstance::AnimNotify_N_Stop_R()
{
	PlayTransition(CreateTransitionMontageStruct());
}

void UBaseLocomotionAnimInstance::AnimNotify_N_Stop_L()
{
	PlayTransition(CreateTransitionMontageStruct());
}

void UBaseLocomotionAnimInstance::AnimNotify_CLF_Stop()
{
	PlayTransition(CreateTransitionMontageStruct());
}

void UBaseLocomotionAnimInstance::AnimNotify_N_QuickStop()
{
	PlayTransition(CreateTransitionMontageStruct());
}

void UBaseLocomotionAnimInstance::AnimNotify_RollToIdle()
{
	PlayTransition(CreateTransitionMontageStruct());
}

void UBaseLocomotionAnimInstance::AnimNotify_LandToIdle()
{
	PlayTransition(CreateTransitionMontageStruct());
}

void UBaseLocomotionAnimInstance::AnimNotify_BowMakeReady()
{
	if (CanOverlayTransition()) {
		PlayTransition(CreateTransitionMontageStruct());
	}
}

void UBaseLocomotionAnimInstance::AnimNotify_BowMakeRelaxed()
{
	if (CanOverlayTransition()) {
		PlayTransition(CreateTransitionMontageStruct());
	}
}

void UBaseLocomotionAnimInstance::AnimNotify_Pivot()
{
	bPivot = CharValues.CharValues.Speed < TiggerPivotSpeedLimit;
	FTimerHandle PivotHandle;
	GetWorld()->GetTimerManager().SetTimer(PivotHandle, [this]() {
		bPivot = false;
	}, .1f, false);
}

void UBaseLocomotionAnimInstance::AnimNotify_Hips_F()
{
	TrackedHipsDirection = EHipsDirection::F;
}

void UBaseLocomotionAnimInstance::AnimNotify_Hips_B()
{
	TrackedHipsDirection = EHipsDirection::B;
}

void UBaseLocomotionAnimInstance::AnimNotify_Hips_LF()
{
	TrackedHipsDirection = EHipsDirection::LF;
}

void UBaseLocomotionAnimInstance::AnimNotify_Hips_LB()
{
	TrackedHipsDirection = EHipsDirection::LB;
}

void UBaseLocomotionAnimInstance::AnimNotify_Hips_RF()
{
	TrackedHipsDirection = EHipsDirection::RF;
}

void UBaseLocomotionAnimInstance::AnimNotify_Hips_RB()
{
	TrackedHipsDirection = EHipsDirection::RB;
}

void UBaseLocomotionAnimInstance::AnimNotify_ResetGroundedEntryState()
{
	GroundedEntryState = EGroundedState::NONE;
}

void UBaseLocomotionAnimInstance::UpdateAimingValues()
{
	FRotator SmoothedAimingRotation;
	SmoothedAimingRotation = UKismetMathLibrary::RInterpTo(SmoothedAimingRotation, CharValues.CharValues.AimingRotation, DeltaTimeX, SmoothedAimingRotationInterpSpeed);
	FRotator TempRot = CharValues.CharValues.AimingRotation - PlayerCharacter->GetActorRotation();
	AimingAngle = FVector2D(TempRot.Yaw, TempRot.Pitch);
	FRotator TempRot_2 = SmoothedAimingRotation - PlayerCharacter->GetActorRotation();
	SmoothedAimingAngle = FVector2D(TempRot_2.Yaw, TempRot_2.Pitch);
	switch (CharValues.MovementStates.RotationMode)
	{
	default:
		break;
	case ERotationMode::LOOKING_DIRECTION:
	case ERotationMode::AIMING:
		AimSweepTime = UKismetMathLibrary::MapRangeClamped(AimingAngle.Y, -90.f, 90.f, 1.f, 0.f);
		SpineRotation.Yaw = AimingAngle.X / 4.f;
		break;
	}

	switch (CharValues.MovementStates.RotationMode)
	{
	default:
		break;
	case ERotationMode::VELOCITY_DIRECTION:
		if (CharValues.CharValues.bHasMovementInput) {
			float PreClampedYaw = (UKismetMathLibrary::Conv_VectorToRotator(CharValues.CharValues.MovementInput) - PlayerCharacter->GetActorRotation()).Yaw;
			float ClampedYaw = UKismetMathLibrary::MapRangeClamped(PreClampedYaw, -180.f, 180.f, 0.f, 1.f);
			InputYawOffsetTime = UKismetMathLibrary::FInterpTo(InputYawOffsetTime, ClampedYaw, DeltaTimeX, InputYawOffsetInterpTime);
		}
		break;
	}
	float AbsSmoothedAngleX = UKismetMathLibrary::Abs(SmoothedAimingAngle.X);
	LeftYawTime = UKismetMathLibrary::MapRangeClamped(AbsSmoothedAngleX, 0.f, 180.f, .5f, 0.f);
	RightYawTime = UKismetMathLibrary::MapRangeClamped(AbsSmoothedAngleX, 0.f, 180.f, .5f, 1.f);
	ForwardYawTime = UKismetMathLibrary::MapRangeClamped(SmoothedAimingAngle.X, -180.f, 180.f, 0.f, 1.f);

}

void UBaseLocomotionAnimInstance::UpdateLayerValues()
{
	Enable_AimOffset = GetCurveValue("Mask_AimOffset");
	BasePose_N = GetCurveValue("BasePose_N");
	BasePose_CLF = GetCurveValue("BasePose_CLF");
	Spine_Add = GetCurveValue("Layering_Spine_Add");
	Head_Add = GetCurveValue("Layering_Head_Add");
	Arm_L_Add = GetCurveValue("Layering_Arm_L_Add");
	Arm_R_Add = GetCurveValue("Layering_Arm_R_Add");
	Hand_R = GetCurveValue("Layering_Hand_R");
	Hand_L = GetCurveValue("Layering_Hand_L");
	Enable_HandIK_L = UKismetMathLibrary::Lerp(0.f, GetCurveValue("Enable_HandIK_L"), GetCurveValue("Layering_Arm_L"));
	Enable_HandIK_R = UKismetMathLibrary::Lerp(0.f, GetCurveValue("Enable_HandIK_R"), GetCurveValue("Layering_Arm_R"));
	Arm_L_LS = GetCurveValue("Layering_Arm_L_LS");
	Arm_L_MS = (1 - UKismetMathLibrary::FFloor(Arm_L_LS));
	Arm_R_LS = GetCurveValue("Layering_Arm_R_LS");
	Arm_R_MS = (1 - UKismetMathLibrary::FFloor(Arm_R_LS));
}

void UBaseLocomotionAnimInstance::UpdateFootIK()
{
	SetFootLocking(ENABLE_FOOTIK_L, FOOTLOCK_L, IK_FOOT_L, FootLock_L_Alpha, FootLock_L_Location, FootLock_L_Rotation);
	SetFootLocking(ENABLE_FOOTIK_R, FOOTLOCK_R, IK_FOOT_R, FootLock_R_Alpha, FootLock_R_Location, FootLock_R_Rotation);
	
}

void UBaseLocomotionAnimInstance::UpdateInAirValues()
{
	FallSpeed = CharValues.CharValues.Velocity.Z;
	LandPrediction = CalculateLandPrediction();
	LeanAmount = InterpLeanAmount(LeanAmount, CalculateInAirLeanAmount(), InAirLeanInterpAmount, DeltaTimeX);
}

void UBaseLocomotionAnimInstance::UpdateRagdollValues()
{
	FlailRate = UKismetMathLibrary::MapRangeClamped(GetOwningComponent()->GetPhysicsLinearVelocity(ROOT).Size(), 0.f, 1000.f, 0.f, 1.f);
}

void UBaseLocomotionAnimInstance::SetFootLocking(FName EnableFootIDCurve, FName FootLockCurve, FName IKFootBone, float CurrentFootLockAlpha, FVector& CurrentFootLockLocation, FRotator& CurrentFootLockRotation)
{
	if (GetCurveValue(EnableFootIDCurve) > 0.f) {
		float FootLockCurveValue = GetCurveValue(FootLockCurve);
		if (FootLockCurveValue >= .99f || FootLockCurveValue < CurrentFootLockAlpha) {
			CurrentFootLockAlpha = FootLockCurveValue;
		}
		if (CurrentFootLockAlpha >= .99f) {
			FTransform TempTrans = PlayerCharacter->GetMesh()->GetSocketTransform(IKFootBone);
			CurrentFootLockLocation = TempTrans.GetLocation();
			CurrentFootLockRotation = TempTrans.GetRotation().Rotator();
		}
		if (CurrentFootLockAlpha > 0.f) {
			SetFootLockOffsets(CurrentFootLockRotation, CurrentFootLockLocation);
		}
	}
}

void UBaseLocomotionAnimInstance::SetFootLockOffsets(FRotator& InRotation, FVector& InLocation)
{
	FRotator RotationDifference;
	FVector LocationDifference;
	if (PlayerCharacter->GetCharacterMovement()->IsMovingOnGround()) {
		RotationDifference = PlayerCharacter->GetActorRotation() - PlayerCharacter->GetCharacterMovement()->GetLastUpdateRotation();
	}
	LocationDifference = UKismetMathLibrary::Quat_UnrotateVector(GetOwningComponent()->GetComponentRotation().Quaternion(), CharValues.CharValues.Velocity * PlayerCharacter->GetWorld()->GetDeltaSeconds());
	FVector TempLoc = InLocation;
	InLocation = UKismetMathLibrary::RotateAngleAxis(TempLoc - LocationDifference, RotationDifference.Yaw, FVector(0.f, 0.f, -1.f));
	FRotator TempRot = InRotation;
	InRotation = (TempRot - RotationDifference);
}

bool UBaseLocomotionAnimInstance::CanOverlayTransition()
{
	return Stance == EAnimStance::STANDING && !bShouldMove;
}

FDynamicMontageParams UBaseLocomotionAnimInstance::CreateTransitionMontageStruct()
{
	FDynamicMontageParams RetParams;
	RetParams.Animation = TransitionSequence;
	RetParams.BlendInTime = .2f;
	RetParams.BlendOutTime = .2f;
	RetParams.PlayRate = 1.5;
	RetParams.StartTime = .4f;
	return RetParams;
}

float UBaseLocomotionAnimInstance::CalculateLandPrediction()
{
	if (FallSpeed < -200.f) {
		UCapsuleComponent* TempCap = PlayerCharacter->GetCapsuleComponent();
		FVector Start = TempCap->GetComponentLocation();
		FVector End = Start + (UKismetMathLibrary::Vector_NormalUnsafe(FVector(CharValues.CharValues.Velocity.X, CharValues.CharValues.Velocity.Y, UKismetMathLibrary::Clamp(CharValues.CharValues.Velocity.Z, -4000.f, -200.f))) * UKismetMathLibrary::MapRangeClamped(CharValues.CharValues.Velocity.Z, 0.f, -4000.f, 50.f, 2000.f));
		float Radius = TempCap->GetScaledCapsuleRadius();
		float HalfHeight = TempCap->GetScaledCapsuleHalfHeight();
		TArray<AActor*> IgnoreActors;
		FHitResult OutHit;
		UKismetSystemLibrary::CapsuleTraceSingle(PlayerCharacter->GetWorld(), Start, End, Radius, HalfHeight, TraceTypeQuery13, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);
		if (PlayerCharacter->GetCharacterMovement()->IsWalkable(OutHit) && OutHit.bBlockingHit) {
			return UKismetMathLibrary::Lerp(LandPredictionCurve->GetFloatValue(OutHit.Time), 0.f, GetCurveValue(MASK_LAND_PREDICTION));
		}
		else {
			return 0.f;
		}
	}
	else {
		return 0.f;
	}
}

FLeanAmountStruct UBaseLocomotionAnimInstance::InterpLeanAmount(FLeanAmountStruct Current, FLeanAmountStruct Target, float InterpSpeed, float DeltaTime)
{
	FLeanAmountStruct ReturnLean;
	ReturnLean.LeftRight = UKismetMathLibrary::FInterpTo(Current.LeftRight, Target.LeftRight, DeltaTime, InterpSpeed);
	ReturnLean.ForwardBack = UKismetMathLibrary::FInterpTo(Current.ForwardBack, Target.ForwardBack, DeltaTime, InterpSpeed);
	return ReturnLean;
}

FLeanAmountStruct UBaseLocomotionAnimInstance::CalculateInAirLeanAmount()
{
	FVector TempVector = UKismetMathLibrary::Quat_UnrotateVector(PlayerCharacter->GetActorRotation().Quaternion(), CharValues.CharValues.Velocity) / 350.f;
	FVector2D TwoDVec = FVector2D(TempVector.Y, TempVector.X) * LeanInAirCurve->GetFloatValue(FallSpeed);
	FLeanAmountStruct RetLean;
	RetLean.ForwardBack = TwoDVec.Y;
	RetLean.LeftRight = TwoDVec.X;
	return RetLean;
}

float UBaseLocomotionAnimInstance::CalculateDiagonalScaleAmount()
{
	return DiagonalScaleAmountCurve->GetFloatValue(UKismetMathLibrary::Abs(VelocityBlend.Front + VelocityBlend.Back));
}

float UBaseLocomotionAnimInstance::CalculateWalkRunBlend()
{
	switch (CharValues.MovementStates.Gait)
	{
	default:
		break;
	case EGait::WALKING:
		return 0.f;
		break;
	case EGait::RUNNING:
	case EGait::SPRINTING:
		return 1.f;
		break;
	}
	return 1.f;
}

float UBaseLocomotionAnimInstance::CalculateStrideBlend()
{
	float TempVal = UKismetMathLibrary::Lerp(StrideBlend_N_Walk->GetFloatValue(CharValues.CharValues.Speed), StrideBlend_N_Run->GetFloatValue(CharValues.CharValues.Speed), GetAnimCurveClamp(WEIGHT_GAIT));
	return UKismetMathLibrary::Lerp(TempVal, StrideBlend_C_Walk->GetFloatValue(CharValues.CharValues.Speed), GetCurveValue(BASE_POSE_CLF));
}

float UBaseLocomotionAnimInstance::CalculateStandingPlayRate()
{
	float LerpA = CharValues.CharValues.Speed / AnimatedWalkSpeed;
	float LerpB = CharValues.CharValues.Speed / AnimatedRunSpeed;
	float WalkRunLerp = UKismetMathLibrary::Lerp(LerpA, LerpB, GetAnimCurveClamp(WEIGHT_GAIT));
	float SprintLerp = UKismetMathLibrary::Lerp(WalkRunLerp, CharValues.CharValues.Speed / AnimatedSprintSpeed, GetAnimCurveClamp(WEIGHT_GAIT, -2.0)) / StrideBlend;
	return UKismetMathLibrary::Clamp(SprintLerp / GetOwningComponent()->K2_GetComponentScale().Z, 0.f, 3.f);
}

float UBaseLocomotionAnimInstance::CalculateCrouchingPlayRate()
{
	float TempVal = ((CharValues.CharValues.Speed / AnimatedCrouchSpeed) / StrideBlend) / GetOwningComponent()->K2_GetComponentScale().Z;
	return UKismetMathLibrary::Clamp(TempVal, 0.f, 2.f);
}

EMovementDirection UBaseLocomotionAnimInstance::CalculateMovementDirection()
{
	switch (CharValues.MovementStates.Gait)
	{
	default:
		break;
	case EGait::WALKING:
	case EGait::RUNNING:
		switch (CharValues.MovementStates.RotationMode)
		{
		default:
			break;
		case ERotationMode::VELOCITY_DIRECTION:
			return EMovementDirection::FORWARD;
			break;
		case ERotationMode::LOOKING_DIRECTION:
		case ERotationMode::AIMING:
			float TempVal = (UKismetMathLibrary::Conv_VectorToRotator(CharValues.CharValues.Velocity) - CharValues.CharValues.AimingRotation).Yaw;
			return CalculateQuadrant(MovementDirection, 70.f, -70.f, 110.f, -110.f, 50.f, TempVal);
			break;
		}
		break;
	case EGait::SPRINTING:
		return EMovementDirection::FORWARD;
		break;
	}
	return EMovementDirection::FORWARD;
}

void UBaseLocomotionAnimInstance::TurnInPlace(FRotator InTargetRotation, float PlayRateScale, float StartTime, bool bOverrideCurrent)
{
	float TurnAngle = (InTargetRotation - PlayerCharacter->GetActorRotation()).Yaw;
	FTurnInPlaceAsset TargetTurnAsset;
	if (UKismetMathLibrary::Abs(TurnAngle) < Turn180Threshold) {
		if (TurnAngle < 0.f) {
			switch (CharValues.MovementStates.Stance)
			{
			default:
				break;
			case EStance::STANDING:
				TargetTurnAsset = N_TurnIP_L90;
				break;
			case EStance::CROUCHING:
				TargetTurnAsset = CLF_TurnIP_L90;
				break;
			}
		}
		else {
			switch (CharValues.MovementStates.Stance)
			{
			default:
				break;
			case EStance::STANDING:
				TargetTurnAsset = N_TurnIP_R90;
				break;
			case EStance::CROUCHING:
				TargetTurnAsset = CLF_TurnIP_R90;
				break;
			}
		}
	}
	else {
		if (TurnAngle < 0.f) {
			switch (CharValues.MovementStates.Stance)
			{
			default:
				break;
			case EStance::STANDING:
				TargetTurnAsset = N_TurnIP_L180;
				break;
			case EStance::CROUCHING:
				TargetTurnAsset = CLF_TurnIP_L180;
				break;
			}
		}
		else {
			switch (CharValues.MovementStates.Stance)
			{
			default:
				break;
			case EStance::STANDING:
				TargetTurnAsset = N_TurnIP_R180;
				break;
			case EStance::CROUCHING:
				TargetTurnAsset = CLF_TurnIP_R180;
				break;
			}
		}
	}
	if (bOverrideCurrent || !IsPlayingSlotAnimation(TargetTurnAsset.Animation, TargetTurnAsset.SlotName)) {
		PlaySlotAnimationAsDynamicMontage(TargetTurnAsset.Animation, TargetTurnAsset.SlotName, .2f, .2f, PlayRateScale * TargetTurnAsset.PlayRate, 1, 0.f, StartTime);
		if (TargetTurnAsset.bScaleTurnAngle) {
			RotationScale = (TurnAngle / TargetTurnAsset.AnimatedAngle) * TargetTurnAsset.PlayRate * PlayRateScale;
		}
		else {
			RotationScale = PlayRateScale * TargetTurnAsset.PlayRate;
		}
	}
}

FVelocityBlendStruct UBaseLocomotionAnimInstance::InterpVelocityBlend(FVelocityBlendStruct Current, FVelocityBlendStruct Target, float InterpSpeed, float DeltaTime)
{
	FVelocityBlendStruct RetVel;
	RetVel.Front = UKismetMathLibrary::FInterpTo(Current.Front, Target.Front, DeltaTime, InterpSpeed);
	RetVel.Back = UKismetMathLibrary::FInterpTo(Current.Back, Target.Back, DeltaTime, InterpSpeed);
	RetVel.Left = UKismetMathLibrary::FInterpTo(Current.Left, Target.Left, DeltaTime, InterpSpeed);
	RetVel.Right = UKismetMathLibrary::FInterpTo(Current.Right, Target.Right, DeltaTime, InterpSpeed);
	return RetVel;
}

FVelocityBlendStruct UBaseLocomotionAnimInstance::CalculateVelocityBlend()
{
	FVector TempRelativeVelocityDir = UKismetMathLibrary::Normal(CharValues.CharValues.Velocity, 0.1f);
	TempRelativeVelocityDir = UKismetMathLibrary::Quat_UnrotateVector(PlayerCharacter->GetActorRotation().Quaternion(), TempRelativeVelocityDir);
	float Sum = UKismetMathLibrary::Abs(TempRelativeVelocityDir.X) + UKismetMathLibrary::Abs(TempRelativeVelocityDir.Y) + UKismetMathLibrary::Abs(TempRelativeVelocityDir.Z);
	FVector RelativeDirection = TempRelativeVelocityDir / Sum;
	FVelocityBlendStruct RetVel;
	RetVel.Front = UKismetMathLibrary::Clamp(RelativeDirection.X, 0.f, 1.f);
	RetVel.Back = UKismetMathLibrary::Clamp(RelativeDirection.X, -1.f, 0.f);
	RetVel.Left = UKismetMathLibrary::Clamp(RelativeDirection.Y, -1.f, 0.f);
	RetVel.Right = UKismetMathLibrary::Clamp(RelativeDirection.Y, 0.f, 1.f);
	return RetVel;
}

FVector UBaseLocomotionAnimInstance::CalculateRelativeAccelerationAmount()
{
	FVector ReturnVector;
	float Catalyst = 0.f;
	if (FVector::DotProduct(CharValues.CharValues.Acceleration, CharValues.CharValues.Velocity) > 0.f) {
		Catalyst = PlayerCharacter->GetCharacterMovement()->GetMaxAcceleration();
	}
	else {
		Catalyst = PlayerCharacter->GetCharacterMovement()->GetMaxBrakingDeceleration();
	}
	ReturnVector = UKismetMathLibrary::LessLess_VectorRotator(UKismetMathLibrary::Vector_ClampSizeMax(CharValues.CharValues.Acceleration, Catalyst) / Catalyst , PlayerCharacter->GetActorRotation());
	return ReturnVector;
}

EMovementDirection UBaseLocomotionAnimInstance::CalculateQuadrant(EMovementDirection Current, float FR_Threshold, float FL_Threshold, float BR_Threshold, float BL_Threshold, float Buffer, float Angle)
{
	if (IsAngleInRange(Angle, FL_Threshold, FR_Threshold, Buffer, Current != EMovementDirection::FORWARD || Current != EMovementDirection::BACKWARD)) {
		return EMovementDirection::FORWARD;
	}
	if (IsAngleInRange(Angle, FR_Threshold, BR_Threshold, Buffer, Current != EMovementDirection::RIGHT || Current != EMovementDirection::LEFT)) {
		return EMovementDirection::RIGHT;
	}
	if (IsAngleInRange(Angle, BL_Threshold, FL_Threshold, Buffer, Current != EMovementDirection::RIGHT || Current != EMovementDirection::LEFT)) {
		return EMovementDirection::LEFT;
	}
	else
	{
		return EMovementDirection::BACKWARD;
	}
	return EMovementDirection::FORWARD;
}

bool UBaseLocomotionAnimInstance::IsAngleInRange(float Angle, float MinAngle, float MaxAngle, float Buffer, bool bIncreaseBuffer)
{
	if(bIncreaseBuffer)
		return UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle - Buffer, MaxAngle + Buffer, true, true);
	return	UKismetMathLibrary::InRange_FloatFloat(Angle, MinAngle + Buffer, MaxAngle - Buffer, true, true);;
}

bool UBaseLocomotionAnimInstance::ShouldMoveCheck()
{
	return (CharValues.CharValues.bIsMoving && CharValues.CharValues.bHasMovementInput) || CharValues.CharValues.Speed > 150.f;
}

void UBaseLocomotionAnimInstance::UpdateMovementValues()
{
	VelocityBlend = InterpVelocityBlend(VelocityBlend, CalculateVelocityBlend(), VelocityBlendInterpSpeed, DeltaTimeX);
	DiagonalScaleAmount = CalculateDiagonalScaleAmount();
	RelativeAccelerationAmount = CalculateRelativeAccelerationAmount();
	FLeanAmountStruct TempLean;
	TempLean.ForwardBack = RelativeAccelerationAmount.X;
	TempLean.LeftRight = RelativeAccelerationAmount.Y;
	LeanAmount = InterpLeanAmount(LeanAmount, TempLean, GroundedLeanInterpAmount, DeltaTimeX);
	WalkRunBlend = CalculateWalkRunBlend();
	StrideBlend = CalculateStrideBlend();
	StandingPlayRate = CalculateStandingPlayRate();
	CrouchingPlayRate = CalculateCrouchingPlayRate();
}

void UBaseLocomotionAnimInstance::UpdateRotationValues()
{
	MovementDirection = CalculateMovementDirection();
	float TempVal = (UKismetMathLibrary::Conv_VectorToRotator(CharValues.CharValues.Velocity) - PlayerCharacter->GetControlRotation()).Yaw;
	FVector FBV = YawOffset_FB->GetVectorValue(TempVal);
	FYaw = FBV.X;
	BYaw = FBV.Y;
	FVector LRV = YawOffset_LR->GetVectorValue(TempVal);
	LYaw = FBV.X;
	RYaw = FBV.Y;
}


bool UBaseLocomotionAnimInstance::CanRotateInPlace()
{
	return CharValues.MovementStates.RotationMode == ERotationMode::AIMING || CharValues.MovementStates.ViewMode == EViewMode::FIRST_PERSON;
}

bool UBaseLocomotionAnimInstance::CanTurnInPlace()
{
	return CharValues.MovementStates.RotationMode == ERotationMode::LOOKING_DIRECTION && CharValues.MovementStates.ViewMode == EViewMode::THIRD_PERSON && GetCurveValue(ENABLE_TRANSITION) > .99;
}

bool UBaseLocomotionAnimInstance::CanDynamicTransition()
{
	return GetCurveValue(ENABLE_TRANSITION) == 1.0f;
}

void UBaseLocomotionAnimInstance::RotateInPlaceCheck()
{
	bRotate_L = AimingAngle.X < RotateMinThreshold;
	bRotate_R = AimingAngle.X > RotateMaxThreshold;
	if (bRotate_L || bRotate_R) {
		RotateRate = UKismetMathLibrary::MapRangeClamped(CharValues.CharValues.AimYawRate, AimYawRateMinRange, AimYawRateMaxRange, MinPlayRate, MaxPlayRate);
	}
}

void UBaseLocomotionAnimInstance::TurnInPlaceCheck()
{
	if (UKismetMathLibrary::Abs(AimingAngle.X) > TurnCheckMinAngle && CharValues.CharValues.AimYawRate < AimYawRateLimit) {
		ElapsedDelayTime += DeltaTimeX;
		float TempVal = UKismetMathLibrary::MapRangeClamped(UKismetMathLibrary::Abs(AimingAngle.X), TurnCheckMinAngle, 180.f, MinAngleDelay, MaxAngleDelay);
		if (ElapsedDelayTime > TempVal) {
			TurnInPlace(FRotator(0.f, 0.f, CharValues.CharValues.AimingRotation.Yaw), 1.0f, 0.f, false);
		}
	}
	else {
		ElapsedDelayTime = 0.f;
		return;
	}
}

void UBaseLocomotionAnimInstance::DynamicTransitionCheck()
{
	if (GetDistanceBetweenTwoSockets(GetOwningComponent(), IK_FOOT_L, RTS_Component, VB_FOOT_TARGET_L, RTS_Component, false, 0.f, 0.f, 0.f, 0.f) > 8.f) {
		PlayDynamicTransition(.1f, CreateTransitionMontageStruct());
	} 
	if (GetDistanceBetweenTwoSockets(GetOwningComponent(), IK_FOOT_R, RTS_Component, VB_FOOT_TARGET_R, RTS_Component, false, 0.f, 0.f, 0.f, 0.f) > 8.f) {
		PlayDynamicTransition(.1f, CreateTransitionMontageStruct());
	}
}

float UBaseLocomotionAnimInstance::GetAnimCurveClamp(FName Name, float Bias, float ClampMin, float ClampMax)
{
	return UKismetMathLibrary::Clamp(GetCurveValue(Name) + Bias, ClampMin, ClampMax);
}
