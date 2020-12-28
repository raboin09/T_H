// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterStatusWidgetData.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStatusWidgetDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:

};

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API UCharacterStatusWidgetData : public UObject
{
	GENERATED_BODY()
	
};
