// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Summoned/BaseTimedSummonedActor.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "Components/CapsuleComponent.h"
#include "BaseSentryTimedSummonedActor.generated.h"

class ABaseProjectile;

UCLASS()
class TACTICSHEROICS_API ABaseSentryTimedSummonedActor : public ABaseTimedSummonedActor
{
	GENERATED_BODY()

public:
	ABaseSentryTimedSummonedActor();

	void Activate() override;

	virtual UShapeComponent* GetCollisionComponent() override;

protected:

	UFUNCTION(BlueprintImplementableEvent)
		void HandleSpawnProjectileFX(FVector FXSpawnLocation);

protected:
	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly)
		float SpawnZMin;
	UPROPERTY(EditDefaultsOnly)
		float SpawnZMax;
	UPROPERTY(EditDefaultsOnly)
		float SpawnBase;

	UPROPERTY(EditDefaultsOnly)
		float ShootCooldown;

	UPROPERTY(EditDefaultsOnly)
		float SpawnOffset;

	UPROPERTY(EditDefaultsOnly)
		float PitchMin;

	UPROPERTY(EditDefaultsOnly)
		float PitchMax;

	UPROPERTY(EditDefaultsOnly)
		float RollMin;

	UPROPERTY(EditDefaultsOnly)
		float RollMax;

	UPROPERTY(EditDefaultsOnly)
		float YawMin;

	UPROPERTY(EditDefaultsOnly)
		float YawMax;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABaseProjectile> ProjectileClass;

private:
	void InitiateFiringLoop();
	void Fire();
	ABaseProjectile* SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation);

private:
};
