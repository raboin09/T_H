// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/OverlapActors/BaseOverlapCombatActor.h"
#include "Components/CapsuleComponent.h"
#include "BaseWeaponActor.generated.h"

UCLASS()
class TACTICSHEROICS_API ABaseWeaponActor : public ABaseOverlapCombatActor
{
	GENERATED_BODY()
	
public:	
	ABaseWeaponActor();

	virtual UShapeComponent* GetCollisionComponent() override { return CollisionComponent; };

	UFUNCTION(BlueprintCallable)
		virtual float GetRange() { return Range; };

protected:
	virtual void BeginPlay() override;
	virtual void HandleOverlapEvent(FGameplayEventData EventData) override;

	UPROPERTY(EditDefaultsOnly)
		UCapsuleComponent* CollisionComponent;

	UPROPERTY(EditDefaultsOnly)
		float Range;

private:

};
