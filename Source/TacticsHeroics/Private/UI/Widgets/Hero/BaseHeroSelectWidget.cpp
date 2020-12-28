#include "BaseHeroSelectWidget.h"
#include "Player/BasePlayerController.h"
#include "Game/SurvivalGameState.h"
#include "Game/BaseTeam.h"
#include "Actors/BaseCharacter.h"
#include "Actors/Objectives/BaseObjectiveActor.h"
#include "UI/Data/Hero/CharacterSelectionWidgetData.h"

void UBaseHeroSelectWidget::SetupHeroSelectionWidget(FCharacterSelectionWidgetDataStruct InSelectionData)
{
	HeroSelectionDataStruct = InSelectionData;
	OnHeroSelectionWidgetSet();
}

FCharacterSelectionWidgetDataStruct UBaseHeroSelectWidget::GetHeroSelectionWidgetData() const
{
	return HeroSelectionDataStruct;
}

void UBaseHeroSelectWidget::HandleHeroButtonClick(FCharacterSelectionWidgetDataStruct InData, FString HeroKey) const
{
	ABasePlayerController* CurrController = Cast<ABasePlayerController>(GetOwningPlayer());
	if (CurrController) {
		FHeroSelectionCard* SelectedCard = InData.HeroCards.Find(HeroKey);
		if (SelectedCard) {
			ASurvivalGameState* CurrGameState = GetWorld()->GetGameState<ASurvivalGameState>();
			if (CurrGameState) {
				TArray<ABaseObjectiveActor*> OwnedObjs = CurrGameState->FindAllObjectivesForTeam(CurrController->GetOwnerTeam());
				AActor* ObjActor = Cast<AActor>(OwnedObjs[0]);
				ABaseCharacter* SpawnedChar = CurrController->GetOwnerTeam()->SpawnCharacterAtActor(Cast<AActor>(OwnedObjs[0]), SelectedCard->HeroClass, false);
				APawn* TempPawn = Cast<APawn>(SpawnedChar);
				CurrController->Possess(TempPawn);
			}
		}
	}
}
