// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseGameplayAbility.h"
#include "SpawnSingleProjectileAbility.generated.h"

class ABaseProjectile;
class ABaseSummonedActor;

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API USpawnSingleProjectileAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

	UPROPERTY(EditDefaultsOnly)
		FName SocketNameToSpawnAt;

protected:

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABaseSummonedActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
		float PitchAdjust;

	UPROPERTY(EditDefaultsOnly)
		bool bSpawnedActorHasVelocity;

	UPROPERTY(EditDefaultsOnly)
		float SpawnOffset;

private:

	ABaseSummonedActor* SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation);
};
