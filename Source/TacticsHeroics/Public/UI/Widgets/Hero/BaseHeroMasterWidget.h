#pragma once

#include "CoreMinimal.h"
#include "UI/Data/Hero/CharacterAbilityWidgetData.h"
#include "UI/Data/Hero/CharacterStatsWidgetData.h"
#include "UI/Data/Hero/CharacterSelectionWidgetData.h"
#include "UI/Data/Hero/CharacterStatusWidgetData.h"
#include "Blueprint/UserWidget.h"
#include "BaseHeroMasterWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TACTICSHEROICS_API UBaseHeroMasterWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MasterWidget")
		void SetupMasterHeroWidget();

	UFUNCTION(BlueprintCallable, Category = "MasterWidget")
		void TeardownMasterHeroWidget();

	UFUNCTION(BlueprintCallable, Category = "MasterWidget")
		void SetupCharacterSelectionWidget();

	UFUNCTION(BlueprintCallable, Category = "MasterWidget")
		void TeardownCharacterSelectionWidget();



	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		FCharacterStatsWidgetDataStruct GetCharacterStatsDataStruct() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		FCharacterCombatAbilitiesWidgetDataStruct GetCharacterCombatAbilitiesDataStruct() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		FCharacterStatusWidgetDataStruct GetCharacterStatusDataStruct() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		FCharacterSelectionWidgetDataStruct GetCharacterSelectionDataStruct() const;



	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void GenerateCharacterStatsWidget(FCharacterStatsWidgetDataStruct InCharacterStatsData);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void GenerateCharacterAbilityWidget(FCharacterCombatAbilitiesWidgetDataStruct InCombatData);
	
	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void GenerateCharacterStatusWidget(FCharacterStatusWidgetDataStruct InCharacterStatusData);

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void GenerateCharacterSelectionWidget(FCharacterSelectionWidgetDataStruct InCharacterStatusData);



	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void OnCharacterStatsDataSetup();

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void OnCharacterAbilityDataSetup();

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void OnCharacterStatusDataSetup();

	UFUNCTION(BlueprintImplementableEvent, Category = "MasterWidget")
		void OnCharacterSelectionDataSetup();

private:

};
