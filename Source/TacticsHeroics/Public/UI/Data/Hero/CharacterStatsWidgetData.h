// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterStatsWidgetData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatsWidgetDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:

};

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API UCharacterStatsWidgetData : public UObject
{
	GENERATED_BODY()
	
};
