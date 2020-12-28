// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Data/Hero/CharacterStatsWidgetData.h"
#include "BaseHeroStatsWidget.generated.h"

class UCharacterStatsWidgetData;

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TACTICSHEROICS_API UBaseHeroStatsWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "StatsWidget")
		void SetupHeroStatsWidget(FCharacterStatsWidgetDataStruct InStatsData);

	UFUNCTION(BlueprintCallable, Category = "StatsWidget")
		FCharacterStatsWidgetDataStruct GetCharacterStatsWidgetData() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "StatsWidget")
		void OnHeroStatsWidgetSet();

private:

	UPROPERTY(transient)
		FCharacterStatsWidgetDataStruct StatsData;
	
};
