#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "API/Game/BaseTeamInterface.h"
#include "API/Actors/BaseObjectiveInterface.h"
#include "SurvivalGameState.generated.h"

class UBaseTeam;

UCLASS()
class TACTICSHEROICS_API ASurvivalGameState : public AGameState
{
	GENERATED_BODY()

public:

	ASurvivalGameState();
	
	UFUNCTION(BlueprintCallable)
		void InitTeams();

	UFUNCTION(BlueprintCallable)
		void SpawnAI();

	void InitPlayerTeam();
	void InitAITeam();

	UFUNCTION(BlueprintCallable)
		UBaseTeam* GetAITeam() { return AITeam; };

	UFUNCTION(BlueprintCallable)
		UBaseTeam* GetPlayerTeam() { return PlayerTeam; };
	

	TArray<ABaseObjectiveActor*> FindAllObjectivesForTeam(UBaseTeam* InTeam);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Replicated)
		UBaseTeam* PlayerTeam;

	UPROPERTY(Replicated)
		UBaseTeam* AITeam;

	UPROPERTY(Replicated)
		TArray<ABaseObjectiveActor*> ActiveObjectives;
	
};
