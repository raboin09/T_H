// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Types/GameTypes.h"
#include "FactionData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TACTICSHEROICS_API UFactionData : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		FFactionDataStruct GetFactionData() const;

private:

	UPROPERTY(EditDefaultsOnly)
		FFactionDataStruct FactionData;
	
};
