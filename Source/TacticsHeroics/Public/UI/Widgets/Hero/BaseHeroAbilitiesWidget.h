// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Data/Hero/CharacterAbilityWidgetData.h"
#include "BaseHeroAbilitiesWidget.generated.h"
 
class UButton;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, BlueprintType, ClassGroup = UI)
class TACTICSHEROICS_API UBaseHeroAbilitiesWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "StatsWidget")
		void SetupHeroAbiltiesWidget(FCharacterCombatAbilitiesWidgetDataStruct InCombatData);

	UFUNCTION(BlueprintCallable, Category = "StatsWidget")
		FCharacterCombatAbilitiesWidgetDataStruct GetCharacterWeaponWidgetData() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "StatsWidget")
		void OnHeroAbiltiesWidgetSet();

private:

	UButton* GetButtonFromIndex(int InIndex);
	UImage* GetIconFromIndex(int InIndex);

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Weapon_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UTextBlock* Weapon_Text;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Ability_1_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Ability_2_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Ability_3_Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UButton* Ability_4_Button;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Weapon_Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Ability_1_Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Ability_2_Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Ability_3_Icon;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		UImage* Ability_4_Icon;

private:

	UPROPERTY(transient)
		FCharacterCombatAbilitiesWidgetDataStruct CombatData;
	
};
