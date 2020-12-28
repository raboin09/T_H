
#include "LocomotionAnimTypes.h"
#include "Actors/BaseCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void FCharMovementValues::Tick(float DeltaTime)
{
	if (PlayerCharacter) {
		AssignEssentialValues();
		CacheValues();

		switch (EssentialValues.MovementStates.MovementState)
		{
		default:
			break;
		case EMovementState::NONE:
			break;
		case EMovementState::GROUNDED:
			UpdateCharacterMovement();
			UpdateGroundedRotation();
			break;
		case EMovementState::IN_AIR:
			UpdateInAirRotation();
			if (EssentialValues.CharValues.bHasMovementInput)
				// TODO
				// MantleCheck(MantleSystem.FallingTraceSettings);
			break;
		case EMovementState::MANTLING:
			break;
		case EMovementState::RAGDOLL:
			UpdateRagdoll();
			break;
		}
	}
}

void FCharMovementValues::SetPlayerCharacter(ABaseCharacter* InChar)
{
	PlayerCharacter = InChar;
}

void FCharMovementValues::CacheValues()
{
	PreviousVelocity = PlayerCharacter->GetVelocity();
	PreviousAimYaw = PlayerCharacter->GetControlRotation().Yaw;
}

FEssentialValuesStruct FCharMovementValues::GetEssentialValues()
{
	// TODO
	return FEssentialValuesStruct();
}

EGait FCharMovementValues::GetAllowedGait()
{
	switch (EssentialValues.MovementStates.Stance)
	{
	case EStance::STANDING:
		switch (EssentialValues.MovementStates.RotationMode)
		{
		default:
			break;
		case ERotationMode::VELOCITY_DIRECTION:
		case ERotationMode::LOOKING_DIRECTION:
			switch (InputValues.DesiredGait)
			{
			default:
				break;
			case EGait::WALKING:
				return EGait::WALKING;
				break;
			case EGait::RUNNING:
				return EGait::RUNNING;
				break;
			case EGait::SPRINTING:
				if (CanSprint()) {
					return EGait::SPRINTING;
				}
				else {
					return EGait::RUNNING;
				}
				break;
			}
			break;
		case ERotationMode::AIMING:
			switch (InputValues.DesiredGait)
			{
			default:
				break;
			case EGait::WALKING:
				return EGait::WALKING;
				break;
			case EGait::RUNNING:
				return EGait::RUNNING;
				break;
			case EGait::SPRINTING:
				return EGait::RUNNING;
				break;
			}
			break;
		}
		break;
	case EStance::CROUCHING:
		switch (InputValues.DesiredGait)
		{
		default:
			break;
		case EGait::WALKING:
			return EGait::WALKING;
			break;
		case EGait::RUNNING:
		case EGait::SPRINTING:
			return EGait::RUNNING;
			break;
		}
		break;
	default:
		break;
	}
	return EGait::RUNNING;
}

EGait FCharMovementValues::GetActualGait()
{
	if (EssentialValues.CharValues.Speed >= CurrentMovementSettings.RunSpeed + 10.f) {
		switch (AllowedGait)
		{
		case EGait::WALKING:
			return EGait::RUNNING;
			break;
		case EGait::RUNNING:
			return EGait::RUNNING;
			break;
		case EGait::SPRINTING:
			return EGait::SPRINTING;
			break;
		default:
			break;
		}
	}
	else {
		if (EssentialValues.CharValues.Speed >= CurrentMovementSettings.WalkSpeed + 10.f) {
			return EGait::RUNNING;
		}
		else {
			return EGait::WALKING;
		}
	}
	return EGait::RUNNING;
}

void FCharMovementValues::SetGait(EGait InGait)
{
	if (EssentialValues.MovementStates.Gait != InGait) {
		EGait TempGait = EssentialValues.MovementStates.Gait;
		EssentialValues.MovementStates.Gait = InGait;
		PreviousActualGait = TempGait;
	}
}

void FCharMovementValues::UpdateCharacterMovement()
{
	AllowedGait = GetAllowedGait();
	ActualGait = GetActualGait();
	if (ActualGait != EssentialValues.MovementStates.Gait) {
		SetGait(ActualGait);
	}
	UpdateDynamicMovementSettings(AllowedGait);
}

void FCharMovementValues::UpdateDynamicMovementSettings(EGait InAllowedGait)
{
	CurrentMovementSettings = GetTargetMovementSettings();
	switch (AllowedGait)
	{
	case EGait::WALKING:
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentMovementSettings.WalkSpeed;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentMovementSettings.WalkSpeed;
		break;
	case EGait::RUNNING:
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentMovementSettings.RunSpeed;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentMovementSettings.WalkSpeed;
		break;
	case EGait::SPRINTING:
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = CurrentMovementSettings.SprintSpeed;
		PlayerCharacter->GetCharacterMovement()->MaxWalkSpeedCrouched = CurrentMovementSettings.WalkSpeed;
		break;
	}

	PlayerCharacter->GetCharacterMovement()->MaxAcceleration = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).X;
	PlayerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Y;
	PlayerCharacter->GetCharacterMovement()->GroundFriction = CurrentMovementSettings.MovementCurve->GetVectorValue(GetMappedSpeed()).Z;
}

void FCharMovementValues::UpdateGroundedRotation()
{
	switch (EssentialValues.MovementStates.MovementAction)
	{
	default:
		break;
	case EMovementAction::NONE:
		if (CanUpdateMovingRotation()) {
			switch (EssentialValues.MovementStates.RotationMode)
			{
			default:
				break;
			case ERotationMode::VELOCITY_DIRECTION:
				SmoothCharacterRotation(FRotator(0.f, 0.f, EssentialValues.CharValues.LastVelocityRotation.Yaw), 800.f, CalculateGroundedRotationRate());
				break;
			case ERotationMode::LOOKING_DIRECTION:
				switch (EssentialValues.MovementStates.Gait)
				{
				default:
					break;
				case EGait::WALKING:
				case EGait::RUNNING:
					SmoothCharacterRotation(FRotator(0.f, 0.f, (PlayerCharacter->GetControlRotation().Yaw + GetAnimCurveValue(YAW_OFFSET))), 500.f, CalculateGroundedRotationRate());
					break;
				case EGait::SPRINTING:
					SmoothCharacterRotation(FRotator(0.f, 0.f, EssentialValues.CharValues.LastVelocityRotation.Yaw), 500.f, CalculateGroundedRotationRate());
					break;
				}
				break;
			case ERotationMode::AIMING:
				SmoothCharacterRotation(FRotator(0.f, 0.f, PlayerCharacter->GetControlRotation().Yaw), 1000.f, 20.f);
				break;
			}
		}
		else {
			switch (EssentialValues.MovementStates.ViewMode)
			{
			default:
				break;
			case EViewMode::FIRST_PERSON:
				LimitRotation(-100.f, 100.f, 20.f);
			case EViewMode::THIRD_PERSON:
				if (UKismetMathLibrary::Abs(GetAnimCurveValue(ROTATION_AMOUNT)) > .001) {
					float DeltaRotZ = GetAnimCurveValue(ROTATION_AMOUNT) * (PlayerCharacter->GetWorld()->GetDeltaSeconds() / (1.f / 30.f));
					PlayerCharacter->AddActorWorldRotation(FRotator(0.f, 0.f, DeltaRotZ));
					RotationSystem.TargetRotation = PlayerCharacter->GetActorRotation();
				}
				break;
			}
		}
		break;
	case EMovementAction::ROLLING:
		if (EssentialValues.CharValues.bHasMovementInput) {
			SmoothCharacterRotation(FRotator(0.f, 0.f, EssentialValues.CharValues.LastMovementInputRotation.Yaw), 0.f, 2.f);
		}
		break;
	}
}

void FCharMovementValues::UpdateInAirRotation()
{
	switch (EssentialValues.MovementStates.RotationMode)
	{
	default:
		break;
	case ERotationMode::VELOCITY_DIRECTION:
	case ERotationMode::LOOKING_DIRECTION:
		SmoothCharacterRotation(FRotator(0.f, 0.f, RotationSystem.InAirRotation.Yaw), 0.f, 5.f);
		break;
	case ERotationMode::AIMING:
		SmoothCharacterRotation(FRotator(0.f, 0.f, PlayerCharacter->GetControlRotation().Yaw), 0.f, 15.f);
		RotationSystem.InAirRotation = PlayerCharacter->GetActorRotation();
		break;
	}
}

void FCharMovementValues::UpdateRagdoll()
{
	LastRagdollVelocity = PlayerCharacter->GetMesh()->GetPhysicsLinearVelocity(ROOT_BONE);
	PlayerCharacter->GetMesh()->SetAllMotorsAngularDriveParams(UKismetMathLibrary::MapRangeClamped(LastRagdollVelocity.Size(), 0.f, 1000.f, 0.f, 25000.f), 0.f, 0.f, false);
	PlayerCharacter->GetMesh()->SetEnableGravity(LastRagdollVelocity.Z > -4000.f);
	SetActorLocationDuringRagdoll();
}

void FCharMovementValues::SetActorLocationDuringRagdoll()
{
	FVector TargetRagdollLocation = PlayerCharacter->GetMesh()->GetSocketLocation(PELVIS_BONE);
	FRotator TargetRagdollRotation;
	FRotator MeshRot = PlayerCharacter->GetMesh()->GetSocketRotation(PELVIS_BONE);
	bRagdollFaceUp = MeshRot.Roll < 0.f;
	if (bRagdollFaceUp) {
		TargetRagdollRotation = FRotator(0.f, 0.f, MeshRot.Yaw - 180.f);
	}
	else {
		TargetRagdollRotation = FRotator(0.f, 0.f, MeshRot.Yaw);
	}
	FVector End = FVector(TargetRagdollLocation.X, TargetRagdollLocation.Y, TargetRagdollLocation.Z - PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	FHitResult HitResult;
	TArray<AActor*> IgnoreActors;
	bRagdollOnGround = UKismetSystemLibrary::LineTraceSingle(PlayerCharacter->GetWorld(), TargetRagdollLocation, End, TraceTypeQuery_MAX, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, HitResult, true);
	if (bRagdollOnGround) {
		float TempFloat = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - UKismetMathLibrary::Abs(HitResult.ImpactPoint.Z - HitResult.TraceStart.Z);
		FVector TempVector = FVector(TargetRagdollLocation.X, TargetRagdollLocation.Y, TargetRagdollLocation.Z + TempFloat);
		SetActorLocationAndRotation_UpdateTarget(TempVector, TargetRagdollRotation, false, false);
	}
	else {
		SetActorLocationAndRotation_UpdateTarget(TargetRagdollLocation, TargetRagdollRotation, false, false);
	}

}

FHitResult FCharMovementValues::SetActorLocationAndRotation_UpdateTarget(FVector InLoc, FRotator InRot, bool bSweep, bool bTeleport)
{
	RotationSystem.TargetRotation = InRot;
	FHitResult HitResult;
	PlayerCharacter->SetActorLocationAndRotation(InLoc, InRot, bSweep, &HitResult, ETeleportType::None);
	return HitResult;
}

void FCharMovementValues::SmoothCharacterRotation(FRotator Target, float TargetInterpSpeed, float ActorInterpSpeed)
{
	RotationSystem.TargetRotation = UKismetMathLibrary::RInterpTo_Constant(RotationSystem.TargetRotation, Target, PlayerCharacter->GetWorld()->GetDeltaSeconds(), TargetInterpSpeed);
	FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(PlayerCharacter->GetActorRotation(), RotationSystem.TargetRotation, PlayerCharacter->GetWorld()->GetDeltaSeconds(), ActorInterpSpeed);
	PlayerCharacter->SetActorRotation(NewRotation);
}

void FCharMovementValues::LimitRotation(float AimYawMin, float AimYawMax, float InterpSpeed)
{
	float DeltaRot = (PlayerCharacter->GetControlRotation() - PlayerCharacter->GetActorRotation()).Yaw;
	if (!UKismetMathLibrary::InRange_FloatFloat(DeltaRot, AimYawMin, AimYawMax, true, true)) {
		float TargetZ = UKismetMathLibrary::SelectFloat(AimYawMin + PlayerCharacter->GetControlRotation().Yaw, AimYawMax + PlayerCharacter->GetControlRotation().Yaw, DeltaRot > 0.f);
		SmoothCharacterRotation(FRotator(0.f, 0.f, TargetZ), 0.f, InterpSpeed);
	}
}

void FCharMovementValues::MantleStart(float MantleHeight, FTransform MantleLedgeWSTransform, UPrimitiveComponent* MantleLedgeWSComponent, EMantleType MantleType)
{

}

FCharMovementSettingsStruct FCharMovementValues::GetTargetMovementSettings()
{
	switch (EssentialValues.MovementStates.RotationMode)
	{
	default:
		break;
	case ERotationMode::VELOCITY_DIRECTION:
		switch (EssentialValues.MovementStates.Stance)
		{
		default:
			break;
		case EStance::STANDING:
			return MovementData.VelocityDirection.Standing;
			break;
		case EStance::CROUCHING:
			return MovementData.VelocityDirection.Crouching;
			break;
		}
		break;
	case ERotationMode::LOOKING_DIRECTION:
		switch (EssentialValues.MovementStates.Stance)
		{
		default:
			break;
		case EStance::STANDING:
			return MovementData.LookingDirection.Standing;
			break;
		case EStance::CROUCHING:
			return MovementData.LookingDirection.Crouching;
			break;
		}
		break;
	case ERotationMode::AIMING:
		switch (EssentialValues.MovementStates.Stance)
		{
		default:
			break;
		case EStance::STANDING:
			return MovementData.Aiming.Standing;
			break;
		case EStance::CROUCHING:
			return MovementData.Aiming.Crouching;
			break;
		}
		break;
	}
	return MovementData.VelocityDirection.Standing;
}

bool FCharMovementValues::CanSprint()
{
	if (EssentialValues.CharValues.bHasMovementInput) {
		switch (EssentialValues.MovementStates.RotationMode)
		{
		default:
			break;
		case ERotationMode::VELOCITY_DIRECTION:
			return EssentialValues.CharValues.MovementInputAmount > .9;
			break;
		case ERotationMode::LOOKING_DIRECTION:
		{
			float YawDelta = (UKismetMathLibrary::Conv_VectorToRotator(PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration()) - PlayerCharacter->GetControlRotation()).Yaw;
			return EssentialValues.CharValues.MovementInputAmount > .9 && UKismetMathLibrary::Abs(YawDelta) < 50.f;
			break;
		}
		case ERotationMode::AIMING:
			return false;
			break;
		}
	}
	return false;
}

bool FCharMovementValues::MantleCheck(FMantleTraceSettings InTraceSettings)
{
	float ZOffset = InTraceSettings.MaxLedgeHeight + InTraceSettings.MinLedgeHeight / 2.0f;
	FVector Start = GetCapsuleBaseLocation(2.0f) + (GetPlayerMovementInput() * -30.f);
	Start.Z += ZOffset;
	FVector End = Start;
	End += (GetPlayerMovementInput() * InTraceSettings.ReachDistance);
	float Radius = InTraceSettings.ForwardTraceRadius;
	float HalfHeight = ((InTraceSettings.MaxLedgeHeight - InTraceSettings.MinLedgeHeight) / 2.0f) + 1.0f;
	TArray<AActor*> IgnoreActors;
	FHitResult OutHit;
	UKismetSystemLibrary::CapsuleTraceSingle(PlayerCharacter->GetWorld(), Start, End, Radius, HalfHeight, TraceTypeQuery13, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHit, true);

	// TODO InitialOverlap BreakHitResult not there
	FVector InitImpactPoint;
	FVector InitTraceNormal;
	if (!PlayerCharacter->GetCharacterMovement()->IsWalkable(OutHit) && OutHit.bBlockingHit) {
		InitImpactPoint = OutHit.ImpactPoint;
		InitTraceNormal = OutHit.ImpactNormal;
	}
	else {
		return false;
	}

	FVector TempEnd = FVector(InitImpactPoint.X, InitImpactPoint.Y, GetCapsuleBaseLocation(0.f).Z) + (InitTraceNormal + -15.f);
	FVector TempStart = TempEnd + FVector(0.f, 0.f, (InTraceSettings.MaxLedgeHeight + InTraceSettings.DownwardTraceRadius + 1.0));
	FHitResult OutHitSphere;
	UKismetSystemLibrary::SphereTraceSingle(PlayerCharacter->GetWorld(), TempStart, TempEnd, InTraceSettings.DownwardTraceRadius, TraceTypeQuery13, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHitSphere, true);
	
	FVector DownTraceLocation;
	UPrimitiveComponent* HitComponent;
	if (PlayerCharacter->GetCharacterMovement()->IsWalkable(OutHitSphere) && OutHitSphere.bBlockingHit) {
		DownTraceLocation = FVector(OutHitSphere.Location.X, OutHitSphere.Location.Y, OutHitSphere.ImpactPoint.Z);
		HitComponent = OutHitSphere.GetComponent();
	}
	else {
		return false;
	}
	FVector CapsuleLocation = GetCapsuleLocationFromBase(DownTraceLocation, 2.0f);
	FTransform TargetTransform;
	float MantleHeight = 0.f;
	if (CapsuleHasRoomCheck(PlayerCharacter->GetCapsuleComponent(), CapsuleLocation, 0.f, 0.f)) {
		TargetTransform = UKismetMathLibrary::MakeTransform(CapsuleLocation, UKismetMathLibrary::Conv_VectorToRotator(InitTraceNormal * FVector(-1.f, -1.f, 0.f)), FVector(0.f, 0.f, 0.f));
		MantleHeight = (TargetTransform.GetLocation() - PlayerCharacter->GetActorLocation()).Size();
	}
	else {
		return false;
	}

	switch (EssentialValues.MovementStates.MovementState)
	{
	default:
		break;
	case EMovementState::NONE:
	case EMovementState::GROUNDED:
	case EMovementState::MANTLING:
	case EMovementState::RAGDOLL:
		if (MantleHeight > 125.f) {
			MantleType = EMantleType::HIGH_MANTLE;
		}
		else {
			MantleType = EMantleType::LOW_MANTLE;
		}
		break;
	case EMovementState::IN_AIR:
		MantleType = EMantleType::FALLING_CATCH;
		break;
	}
	MantleStart(MantleHeight, TargetTransform, HitComponent, MantleType);
	return true;
}

float FCharMovementValues::GetMappedSpeed()
{
	if (EssentialValues.CharValues.Speed <= CurrentMovementSettings.WalkSpeed) {
		return UKismetMathLibrary::MapRangeClamped(EssentialValues.CharValues.Speed, 0.0f, CurrentMovementSettings.WalkSpeed, 0.0f, 1.0f);
	}
	else if (EssentialValues.CharValues.Speed <= CurrentMovementSettings.RunSpeed) {
		return UKismetMathLibrary::MapRangeClamped(EssentialValues.CharValues.Speed, CurrentMovementSettings.WalkSpeed, CurrentMovementSettings.RunSpeed, 1.0f, 2.0f);
	}
	else {
		return UKismetMathLibrary::MapRangeClamped(EssentialValues.CharValues.Speed, CurrentMovementSettings.RunSpeed, CurrentMovementSettings.SprintSpeed, 2.0f, 3.0f);
	}
	return 1.0f;
}

float FCharMovementValues::GetAnimCurveValue(FName CurveName)
{
	UAnimInstance* AnimInstance = PlayerCharacter->GetMesh()->GetAnimInstance();
	if(AnimInstance) return AnimInstance->GetCurveValue(CurveName);
	return 0.f;
}

FVector FCharMovementValues::GetCapsuleBaseLocation(float ZOffset)
{
	UCapsuleComponent* CapComp = PlayerCharacter->GetCapsuleComponent();
	return CapComp->K2_GetComponentLocation() - (CapComp->GetUpVector() * (CapComp->GetScaledCapsuleHalfHeight() + ZOffset));
}

FVector FCharMovementValues::GetCapsuleLocationFromBase(FVector BaseLocation, float ZOffset)
{
	return BaseLocation + FVector(0.f, 0.f, PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + ZOffset);
}

FVector FCharMovementValues::GetPlayerMovementInput()
{
	FVector ForwardVec = PlayerCharacter->GetInputAxisValue(MOVE_FORWARD_AXIS) * PlayerCharacter->GetActorForwardVector();
	FVector RightVec = PlayerCharacter->GetInputAxisValue(MOVE_RIGHT_AXIS) * PlayerCharacter->GetActorRightVector();
	return UKismetMathLibrary::Normal(ForwardVec + RightVec, .0001);
}

float FCharMovementValues::CalculateGroundedRotationRate()
{
	return CurrentMovementSettings.RotationRateCurve->GetFloatValue(GetMappedSpeed()) * UKismetMathLibrary::MapRangeClamped(EssentialValues.CharValues.AimYawRate, 0.f, 300.f, 1.f, 3.f);
}

bool FCharMovementValues::CapsuleHasRoomCheck(UCapsuleComponent* Capsule, FVector TargetLocation, float HeightOffset, float RadiusOffset)
{
	float TempVal = Capsule->GetScaledCapsuleHalfHeight_WithoutHemisphere() + (RadiusOffset * -1.f) + HeightOffset;
	FVector Start = TargetLocation;
	FVector End = TargetLocation;
	Start.Z += TempVal;
	End.Z -= TempVal;
	float Radius = Capsule->GetScaledCapsuleRadius() + RadiusOffset;
	TArray<AActor*> IgnoreActors;
	FHitResult OutHitResult;
	UKismetSystemLibrary::SphereTraceSingleByProfile(PlayerCharacter->GetWorld(), Start, End, Radius, PAWN_CHANNEL, false, IgnoreActors, EDrawDebugTrace::ForOneFrame, OutHitResult, true);
	// TODO OutHit InitialOverlap is missing
	return (!OutHitResult.bBlockingHit);
}

bool FCharMovementValues::CanUpdateMovingRotation()
{
	return ((EssentialValues.CharValues.bIsMoving && EssentialValues.CharValues.bHasMovementInput) || (EssentialValues.CharValues.Speed > 150.f)) && !PlayerCharacter->HasAnyRootMotion();
}

void FCharMovementValues::AssignEssentialValues()
{
	FVector CurrVelocity = PlayerCharacter->GetVelocity();
	EssentialValues.CharValues.Acceleration = CurrVelocity - PreviousVelocity / PlayerCharacter->GetWorld()->DeltaTimeSeconds;
	EssentialValues.CharValues.bIsMoving = FVector(CurrVelocity.X, CurrVelocity.Y, 0.f).Size() > 1.0f;
	EssentialValues.CharValues.MovementInputAmount = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() / PlayerCharacter->GetCharacterMovement()->GetMaxAcceleration();
	EssentialValues.CharValues.bHasMovementInput = EssentialValues.CharValues.MovementInputAmount > 0;
	if (EssentialValues.CharValues.bIsMoving)
		EssentialValues.CharValues.LastVelocityRotation = UKismetMathLibrary::Conv_VectorToRotator(CurrVelocity);
	if (EssentialValues.CharValues.bHasMovementInput)
		EssentialValues.CharValues.LastMovementInputRotation = UKismetMathLibrary::Conv_VectorToRotator(PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration());
	EssentialValues.CharValues.AimYawRate = UKismetMathLibrary::Abs(PlayerCharacter->GetControlRotation().Yaw - PreviousAimYaw) / PlayerCharacter->GetWorld()->GetDeltaSeconds();

	switch (EssentialValues.MovementStates.MovementState)
	{
	default:
		break;
	case EMovementState::NONE:
		break;
	case EMovementState::GROUNDED:
		AllowedGait = GetAllowedGait();
		break;
	case EMovementState::IN_AIR:
		break;
	case EMovementState::MANTLING:
		break;
	case EMovementState::RAGDOLL:
		break;
	}
}
