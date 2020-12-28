// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "HeroWidgetData.generated.h"

class UBaseHeroMasterWidget;

USTRUCT(BlueprintType)
struct FHeroWidgetUIData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UBaseHeroMasterWidget> MasterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName UIDataKey;
};

UCLASS(BlueprintType)
class TACTICSHEROICS_API UHeroWidgetData : public UDataAsset
{
	GENERATED_BODY()

public:

	FHeroWidgetUIData GetHeroUIDataStruct();

private:

	UPROPERTY(EditDefaultsOnly)
		FHeroWidgetUIData HeroUIData;	
};
