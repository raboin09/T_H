// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "API/Actors/BasePawnInterface.h"
#include "Types/GameTypes.h"
#include "API/Actors/BaseObjectiveInterface.h"
#include "BaseTeamInterface.generated.h"

class ABaseCharacter;
class ABaseGeneralPawn;
class ABaseObjectiveActor;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UBaseTeamInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseTeamInterface
{
	GENERATED_BODY()

		// TODO Redo code to separate the attributes and movement to BaseCharInterface and have BaseGeneralPawn inherit from this

public:
	/*
	virtual void SetFactionData(FFactionDataStruct InFactionData) = 0;
	virtual FFactionDataStruct GetFactionData() = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TScriptInterface<IBasePawnInterface> SpawnCharacterAtLocation(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseCharacter> CharacterClass, bool bShouldSpawnNewController) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual ABaseCharacter* SpawnRandomCharacterAtRandomObjective(bool bShouldSpawnNewController) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TScriptInterface<IBasePawnInterface> SpawnCharacterAtActor(AActor* ActorToSpawnAt, TSubclassOf<ABaseCharacter> CharacterClass, bool bShouldSpawnNewController) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual ABaseGeneralPawn* SpawnGeneralAtLocation(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseGeneralPawn> CharacterClass, bool bShouldSpawnNewController) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void SetOwnerTeam(TScriptInterface<IBaseTeamInterface> InTeam) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TScriptInterface<IBaseTeamInterface> GetOwnerTeam() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void AddObjectiveToTeam(ABaseObjectiveActor* InObjective) = 0;

	//UFUNCTION(BlueprintCallable, Category = "BaseTeam")
		virtual bool IsPawnOnThisTeam(TScriptInterface<IBasePawnInterface> InPawn) = 0;

	virtual void RemoveObjectiveFromTeam(ABaseObjectiveActor* InObjective) = 0;

	virtual int32 GetNumberOfOwnedObjectives() = 0;

	virtual void SetWorld(UWorld* InWorld) = 0;
	virtual UWorld* GetWorld() = 0;
	*/
};
