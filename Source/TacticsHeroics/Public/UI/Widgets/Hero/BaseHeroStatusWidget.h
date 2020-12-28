// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Data/Hero/CharacterStatusWidgetData.h"
#include "BaseHeroStatusWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TACTICSHEROICS_API UBaseHeroStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "StatsWidget")
		void SetupHeroStatusWidget(FCharacterStatusWidgetDataStruct InStatusData);

	UFUNCTION(BlueprintCallable, Category = "StatsWidget")
		FCharacterStatusWidgetDataStruct GetCharacterStatusWidgetData() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "StatsWidget")
		void OnHeroStatusWidgetSet();

private:

	UPROPERTY(transient)
		FCharacterStatusWidgetDataStruct StatusData;
	
};
