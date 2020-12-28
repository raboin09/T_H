// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/BaseGameInstance.h"

void UBaseGameInstance::SetPersistentID(int32 InID)
{
	PersistentID = InID;
}

int32 UBaseGameInstance::GetPersistentID() const
{
	return PersistentID;
}

void UBaseGameInstance::SetTeamData(TMap<int32, FMainMenuTeamStruct> InMap)
{
	TeamData = InMap;
}

TMap<int32, FMainMenuTeamStruct> UBaseGameInstance::GetTeamData() const
{
	return TeamData;
}

TArray<UFactionData*> UBaseGameInstance::GetFactionData()
{
	return FactionData;
}
