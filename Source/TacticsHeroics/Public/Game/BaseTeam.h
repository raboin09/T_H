// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Types/GameTypes.h"
#include "API/Game/BaseTeamInterface.h"
#include "API/Actors/BasePawnInterface.h"
#include "API/Actors/BaseObjectiveInterface.h"
#include "BaseTeam.generated.h"

class ABaseGeneralPawn;

UCLASS()
class TACTICSHEROICS_API UBaseTeam : public UObject
{
	GENERATED_BODY()

public:

	virtual void SetFactionData(FFactionDataStruct InFactionData);
	virtual FFactionDataStruct GetFactionData();

	UFUNCTION(BlueprintCallable, Category = "BaseTeam")
		virtual ABaseCharacter* SpawnCharacterAtLocation(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseCharacter> CharacterClass, bool bShouldSpawnNewController);
	UFUNCTION(BlueprintCallable, Category = "BaseTeam")
		virtual ABaseCharacter* SpawnRandomCharacterAtRandomObjective(bool bShouldSpawnNewController);
	UFUNCTION(BlueprintCallable, Category = "BaseTeam")
		virtual ABaseGeneralPawn* SpawnGeneralAtLocation(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseGeneralPawn> CharacterClass, bool bShouldSpawnNewController);
	UFUNCTION(BlueprintCallable, Category = "BaseTeam")
		virtual ABaseCharacter* SpawnCharacterAtActor(AActor* ActorToSpawnAt, TSubclassOf<ABaseCharacter> CharacterClass, bool bShouldSpawnNewController);
	UFUNCTION(BlueprintCallable)
		virtual void AddObjectiveToTeam(ABaseObjectiveActor* InObjective);
	UFUNCTION(BlueprintCallable)
		virtual bool IsPawnOnThisTeam(APawn* InPawn);
	bool IsPawnOnThisTeam(AActor* InActor);
	UFUNCTION(BlueprintCallable)
		void SetWorld(UWorld* InWorld) { World = InWorld; };
	UFUNCTION(BlueprintCallable)
		UWorld* GetWorld() { return World; };

	FFactionDataStruct GetTeamData() { return TeamData; };

	virtual void RemoveObjectiveFromTeam(ABaseObjectiveActor* InObjective);


	virtual int32 GetNumberOfOwnedObjectives();

private:

	TArray<AController*> ControllerArray;
	TArray<ABaseObjectiveActor*> OwnedObjectives;
	TArray<APawn*> OwnedPawns;
	FFactionDataStruct TeamData;

	UWorld* World;

	int32 TeamID;
	
};
