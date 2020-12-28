// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Types/GameTypes.h"
#include "Data/Game/FactionData.h"
#include "Data/Game/LevelData.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AMainMenuGameMode();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

private:
	FMainMenuGameStruct GameStruct;
	FLevelDataStruct SelectedLevel;
	FFactionDataStruct SelectedFaction;
	TMap<int32, FMainMenuTeamStruct> Teams;
};
