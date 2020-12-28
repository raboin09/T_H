// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/GameTypes.h"
#include "LevelData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TACTICSHEROICS_API ULevelData : public UDataAsset
{
	GENERATED_BODY()

public:

	FLevelDataStruct GetLevelData() const;

private:

	UPROPERTY(EditDefaultsOnly)
		FLevelDataStruct LevelDataStruct;
	
};
