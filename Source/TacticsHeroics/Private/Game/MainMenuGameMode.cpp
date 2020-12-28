// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"
#include "Game/BaseGameInstance.h"
#include "API/Game/BaseGameInstanceInterface.h"

AMainMenuGameMode::AMainMenuGameMode()
{

}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AMainMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}