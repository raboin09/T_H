// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SurvivalGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API ASurvivalGameMode : public AGameMode
{
	GENERATED_BODY()

protected:

	virtual void StartMatch() override;

	UFUNCTION(BlueprintImplementableEvent, Category = SurvivalGameMode)
		void InitTeams();

	UFUNCTION(BlueprintCallable)
		void InitiateAIWave(int32 WaveLevel);

};
