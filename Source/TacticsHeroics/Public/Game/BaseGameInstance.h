// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Data/UI/InGame/HeroWidgetData.h"
#include "Data/Game/FactionData.h"
#include "Game/BaseGameInstanceInterface.h"
#include "BaseGameInstance.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TACTICSHEROICS_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void SetPersistentID(int32 InID);
	virtual int32 GetPersistentID() const;

	virtual void SetTeamData(TMap<int32, FMainMenuTeamStruct> InMap);
	TMap<int32, FMainMenuTeamStruct> GetTeamData() const;

	virtual TArray<UFactionData*> GetFactionData();

public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABaseCharacter> AICharClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABaseCharacter> TEMP_PaladinClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		EPlayerType SelectedRole;
	
	UPROPERTY(EditDefaultsOnly)
		uint32 NumDwarvesToSpawn;

	UPROPERTY(EditDefaultsOnly)
		uint32 NumSkeletonsToSpawn;

private:

	UPROPERTY(EditDefaultsOnly)
		TArray<UFactionData*> FactionData;

	TMap<int32, FMainMenuTeamStruct> TeamData;
	int32 PersistentID;
	
};
