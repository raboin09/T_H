#include "SurvivalGameMode.h"
#include "Player/BasePlayerController.h"
#include "Game/BaseGameInstance.h"
#include "Player/BasePlayerState.h"
#include "Game/SurvivalGameState.h"
#include "Engine/World.h"

void ASurvivalGameMode::StartMatch()
{
	Super::StartMatch();
	InitTeams();
	UE_LOG(LogTemp, Warning, TEXT("BattlefieldGameMode::StartMatch - Starting Match"));
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ABasePlayerController* CurrPlayerController = Cast<ABasePlayerController>(*Iterator);
		ASurvivalGameState* CurrGameState = GetGameState<ASurvivalGameState>();
		if (CurrPlayerController && CurrGameState)
		{
			ABasePlayerState* CurrPlayerState = CurrPlayerController->GetPlayerState<ABasePlayerState>();
			UBaseGameInstance* CurrGameInst = CurrPlayerController->GetGameInstance<UBaseGameInstance>();
			if (CurrGameInst && CurrPlayerState) {				
				CurrPlayerState->SetPlayerType(CurrGameInst->SelectedRole);
				CurrPlayerState->SetTeam(CurrGameState->GetPlayerTeam());
				if (CurrPlayerState->GetPlayerType() == EPlayerType::HERO) {
					CurrPlayerController->InitHeroPlayer();
				}
				else {
					CurrPlayerController->InitGeneralPlayer();
				}
			}
		}
	}
	InitiateAIWave(1);
}

void ASurvivalGameMode::InitiateAIWave(int32 WaveLevel)
{
	ASurvivalGameState* CurrGameState = GetGameState<ASurvivalGameState>();
	if (CurrGameState) {
		WaveLevel = WaveLevel < 1 ? 1 : WaveLevel;
		for (int i = 0; i < WaveLevel; i++) {
			CurrGameState->SpawnAI();
		}
	}
}
