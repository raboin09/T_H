// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/OverlapActors/BaseOverlapCombatActor.h"
#include "Components/SphereComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class TACTICSHEROICS_API ABaseProjectile : public ABaseOverlapCombatActor
{
	GENERATED_BODY()

public:
	ABaseProjectile();

	virtual UShapeComponent* GetCollisionComponent() override;

protected:
	virtual void BeginPlay() override;
	virtual void HandleOverlapEvent(FGameplayEventData EventData) override;

	UPROPERTY(EditDefaultsOnly)
		float DeathBuffer;

	UPROPERTY(EditDefaultsOnly)
		bool bExplodesOnDeath;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* ExplosionRadius;

private:

	void DestroyProjectile();
	
};
