// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Summoned/BaseSummonedActor.h"
#include "BaseTimedSummonedActor.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class TACTICSHEROICS_API ABaseTimedSummonedActor : public ABaseSummonedActor
{
	GENERATED_BODY()

public:

	virtual void Activate() override;
	virtual void OnDeactivation();
	virtual UShapeComponent* GetCollisionComponent() override { return nullptr; };

protected:

	UPROPERTY(EditDefaultsOnly)
		float TimeAlive;

	bool bIsAlive;
	
};
