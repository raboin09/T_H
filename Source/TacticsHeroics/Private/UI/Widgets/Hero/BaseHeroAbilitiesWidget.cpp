// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHeroAbilitiesWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"


void UBaseHeroAbilitiesWidget::SetupHeroAbiltiesWidget(FCharacterCombatAbilitiesWidgetDataStruct InCombatData)
{
	CombatData = InCombatData;
	if (CombatData.WeaponWidgetData.AbilityIcon) {
		Weapon_Icon->SetBrushFromSoftTexture(CombatData.WeaponWidgetData.AbilityIcon);
	}
	
	for (int i = 0; i < 4; i++) {
		if (i >= CombatData.AbilityWidgetData.Num())
			return;
		if (CombatData.AbilityWidgetData[i].AbilityIcon && GetIconFromIndex(i)) {
			GetIconFromIndex(i)->SetBrushFromSoftTexture(CombatData.AbilityWidgetData[i].AbilityIcon);
		}
	}
	OnHeroAbiltiesWidgetSet();
}

FCharacterCombatAbilitiesWidgetDataStruct UBaseHeroAbilitiesWidget::GetCharacterWeaponWidgetData() const
{
	return CombatData;
}

UButton* UBaseHeroAbilitiesWidget::GetButtonFromIndex(int InIndex)
{
	switch (InIndex)
	{
	case -1:
		return Weapon_Button;
		break;
	case 0:
		return Ability_1_Button;
		break;
	case 1:
		return Ability_2_Button;
		break;
	case 2:
		return Ability_3_Button;
		break;
	case 3:
		return Ability_4_Button;
		break;
	default:
		return nullptr;
	}
}

UImage* UBaseHeroAbilitiesWidget::GetIconFromIndex(int InIndex)
{
	switch (InIndex)
	{
	case -1:
		return Weapon_Icon;
		break;
	case 0:
		return Ability_1_Icon;
		break;
	case 1:
		return Ability_2_Icon;
		break;
	case 2:
		return Ability_3_Icon;
		break;
	case 3:
		return Ability_4_Icon;
		break;
	default:
		return nullptr;
	}
}
