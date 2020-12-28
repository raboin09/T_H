// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "API/AI/BaseAIControllerInterface.h"
#include "API/Actors/BasePawnInterface.h"
#include "BaseAIController.generated.h"

class UBehaviorTreeComponent;
class UAIPerceptionComponent;
class UBehaviorTree;
class UBlackboardComponent;
class ABaseCharacter;
class UAISenseConfig_Sight;
class UAIData;
class UBaseTeam;
class ATextRenderActor;

UCLASS()
class TACTICSHEROICS_API ABaseAIController : public AAIController
{
	GENERATED_BODY()

public:

	ABaseAIController();

	virtual void OnPossess(APawn* InPawn);

	UFUNCTION(BlueprintNativeEvent, Category = "BaseAIController")
		void HandlePawnDeath();
	virtual void HandlePawnDeath_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "BaseAIController")
		void SpawnNewPawn();
	virtual void SpawnNewPawn_Implementation();

	UFUNCTION(BlueprintImplementableEvent, Category = "BaseAIController")
		void ChangePawnState(EAIStatus NewStatus);

	virtual ABaseCharacter* GetPossessedPawn();

	UFUNCTION()
		virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	virtual void SetupAIController();
	virtual AActor* GetSeeingPawn();

	virtual void SetOwnerTeam(UBaseTeam* InTeam) { CurrTeam = InTeam; };
	UBaseTeam* GetOwnerTeam() {	return CurrTeam; };

protected:

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly)
		bool bShouldRun;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EAIStatus PawnStatus;

private:

	ABaseCharacter* CurrentPawn;
	AActor* CurrentTarget;
	UBaseTeam* CurrTeam;

	bool bInCombat;
	bool bPossessed;

	TArray<AActor*> VisibleEnemies;

	FName BlackboardEnemyKey;
	FName BlackboardCanSeeEnemyKey;

	FTimerHandle PulseHandler;

	UAISenseConfig_Sight* Sight;
	UAIPerceptionComponent* AIPerceptionComponent;
	UBehaviorTreeComponent* BehaviorTreeComponent;
	UBlackboardComponent* CurrentBlackboard;
};
