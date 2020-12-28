// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "Types/LocomotionAnimTypes.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;



USTRUCT(BlueprintType)
struct FBaseAnimInstanceProxy : public FAnimInstanceProxy
{

    GENERATED_BODY()

    FBaseAnimInstanceProxy()

    : FAnimInstanceProxy()

    {}

    FBaseAnimInstanceProxy(UAnimInstance* Instance);

	virtual void Update(float DeltaSeconds) override;

	void SetAnimInstance(UAnimInstance* InInstance);

	float CalculateLeanAmount();
	float CalculateLeanInterp();

private:
	
	void UpdatePawnOwner();

	UAnimInstance* AnimInstance;
	ABaseCharacter* PlayerCharacter;

public:

	// CHAR VALUES

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bAttacking;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsFalling;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsMoving;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		bool bIsJumping;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float Speed;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float Direction;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float Yaw;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float YawDelta;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float Roll;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float Pitch;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		FRotator LastTickRotation;


	// LOGIC VALUES

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float FinalTurnLeanAmount;

	UPROPERTY(transient, EditAnywhere, BlueprintReadWrite, Category = Animation)
		float TurnLeanOffset = 10.f;

	float TurnLeanAmount;
	float TurnLeanInterpSpeed;

	FName TURN_AXIS = "Turn";
};

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class TACTICSHEROICS_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override;

private:

	UPROPERTY(Transient, BlueprintReadOnly, Category = "BaseAnimInstanceProxy", meta = (AllowPrivateAccess = "true"))
		FBaseAnimInstanceProxy Proxy;

	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override {
		return &Proxy;
	}

	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override {

	}

	friend struct FBaseAnimInstanceProxy;
	
};
