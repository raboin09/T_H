#include "Animation/BaseAnimInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/BaseCharacter.h"

FBaseAnimInstanceProxy::FBaseAnimInstanceProxy(UAnimInstance* Instance)
{

}

void FBaseAnimInstanceProxy::Update(float DeltaSeconds)
{
	UpdatePawnOwner();
	if (PlayerCharacter && AnimInstance) {
		bIsFalling = PlayerCharacter->GetCharacterMovement()->IsFalling();
		Speed = PlayerCharacter->GetVelocity().Size();
		FRotator CharRot = PlayerCharacter->GetActorRotation();
		Direction = UKismetMathLibrary::NormalizedDeltaRotator(PlayerCharacter->GetBaseAimRotation(), UKismetMathLibrary::Conv_VectorToRotator(PlayerCharacter->GetVelocity())).Yaw * -1.f;
		FRotator PlayerRot = LastTickRotation - CharRot;
		Yaw = PlayerRot.Yaw;
		Roll = PlayerRot.Roll;
		Pitch = PlayerRot.Pitch;

		LastTickRotation = CharRot;

		bIsJumping = PlayerCharacter->bWasJumping;
		bIsMoving = PlayerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

		bAttacking = !PlayerCharacter->CanAttack();
		TurnLeanAmount = UKismetMathLibrary::FInterpTo(TurnLeanAmount, CalculateLeanAmount(), PlayerCharacter->GetWorld()->GetDeltaSeconds(), CalculateLeanInterp());
		FinalTurnLeanAmount = TurnLeanAmount * TurnLeanOffset;
	}
}

void FBaseAnimInstanceProxy::SetAnimInstance(UAnimInstance* InInstance)
{
	AnimInstance = InInstance;
}

float FBaseAnimInstanceProxy::CalculateLeanAmount()
{
	if (bIsFalling && Speed > 300) {
		return 0.f;
	}
	else {
		return UKismetMathLibrary::Clamp(PlayerCharacter->GetInputAxisValue(TURN_AXIS), -1.f, 1.f);
	}
	return 0.f;
}

float FBaseAnimInstanceProxy::CalculateLeanInterp()
{
	if (bIsFalling && Speed > 300) {
		return 10.f;
	}
	else {
		return 1.f;
	}
	return 0.f;
}

void FBaseAnimInstanceProxy::UpdatePawnOwner()
{
	if (AnimInstance) {
		if (!PlayerCharacter) {
			PlayerCharacter = Cast<ABaseCharacter>(AnimInstance->TryGetPawnOwner());
		}
	}
}

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Proxy.SetAnimInstance(this);
}
