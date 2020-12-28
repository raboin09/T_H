#include "BaseHeroMasterWidget.h"
#include "UI/Data/Hero/CharacterAbilityWidgetData.h"

void UBaseHeroMasterWidget::SetupMasterHeroWidget()
{
	GenerateCharacterAbilityWidget(GetCharacterCombatAbilitiesDataStruct());
	GenerateCharacterStatsWidget(GetCharacterStatsDataStruct());
	GenerateCharacterStatusWidget(GetCharacterStatusDataStruct());
}

void UBaseHeroMasterWidget::TeardownMasterHeroWidget()
{

}

void UBaseHeroMasterWidget::SetupCharacterSelectionWidget()
{
	GenerateCharacterSelectionWidget(GetCharacterSelectionDataStruct());
}

void UBaseHeroMasterWidget::TeardownCharacterSelectionWidget()
{

}
