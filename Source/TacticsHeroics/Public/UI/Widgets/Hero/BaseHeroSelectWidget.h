// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Data/Hero/CharacterSelectionWidgetData.h"
#include "Blueprint/UserWidget.h"
#include "BaseHeroSelectWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TACTICSHEROICS_API UBaseHeroSelectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "HeroSelectWidget")
		void SetupHeroSelectionWidget(FCharacterSelectionWidgetDataStruct InSelectionData);

	UFUNCTION(BlueprintCallable, Category = "HeroSelectWidget")
		FCharacterSelectionWidgetDataStruct GetHeroSelectionWidgetData() const;

	UFUNCTION(BlueprintCallable, Category = "HeroSelectWidget")
		void HandleHeroButtonClick(FCharacterSelectionWidgetDataStruct InData, FString HeroKey) const;

	UFUNCTION(BlueprintImplementableEvent, Category = "HeroSelectWidget")
		void OnHeroSelectionWidgetSet();

protected:

	UPROPERTY(transient)
		FCharacterSelectionWidgetDataStruct HeroSelectionDataStruct;
};
