// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseCharacter.h"
#include "Game/BaseTeam.h"
#include "Perception/AIPerceptionComponent.h"
#include "TimerManager.h"
#include "Player/BasePlayerState.h"

ABaseAIController::ABaseAIController()
{
	bWantsPlayerState = true;
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorTreeComponent"));
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(FName("BehaviorTree"));
	CurrentBlackboard = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(FName("AIPerceptionComp"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(FName("Sight Config"));
	Sight->SightRadius = 2000.f;
	Sight->LoseSightRadius = 2500.f;
	Sight->PeripheralVisionAngleDegrees = 60.f;
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	BlackboardEnemyKey = "TargetActor";
	BlackboardCanSeeEnemyKey = "CanSeeEnemy";

	AIPerceptionComponent->ConfigureSense(*Sight);
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	FRotator InRot;
	FVector InLoc;
	GetPlayerViewPoint(InLoc, InRot);
	InPawn->SetActorLocationAndRotation(InLoc, InRot);
	InPawn->AddActorLocalRotation(FRotator(0.f, -90.f, 0.f));
	CurrentPawn = Cast<ABaseCharacter>(InPawn);
	ABasePlayerState* CurrentPlayerState = GetPlayerState<ABasePlayerState>();
	bPossessed = true;
	if (CurrentPlayerState)
	{
		CurrentPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CurrentPlayerState, InPawn);
	}
	SetupAIController();
	ChangePawnState(EAIStatus::INACTIVE);
	
}

void ABaseAIController::HandlePawnDeath_Implementation()
{
	UnPossess();
	bPossessed = false;
	FTimerHandle SpawnTimer;
	BehaviorTreeComponent->StopTree();
	ChangePawnState(EAIStatus::UNPOSSESSED);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ABaseAIController::SpawnNewPawn, 5.f, false);
}

void ABaseAIController::SpawnNewPawn_Implementation()
{
	bPossessed = true;
	CurrentPawn = CurrTeam->SpawnRandomCharacterAtRandomObjective(false);
	Possess(CurrentPawn);
}

ABaseCharacter* ABaseAIController::GetPossessedPawn()
{
	return CurrentPawn;
}

void ABaseAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors) {
		ABaseCharacter* CurrPawn = Cast<ABaseCharacter>(Actor);
		if (CurrPawn && CurrentPawn && CurrPawn->IsAlive() && !GetSeeingPawn() && CurrPawn->GetOwnerTeam() != CurrentPawn->GetOwnerTeam())
		{
			//ChangePawnState(EAIStatus::COMBAT);
			CurrentBlackboard->SetValueAsObject(BlackboardEnemyKey, Actor);
		}
		else {
			//ChangePawnState(EAIStatus::INACTIVE);
		}
	}
}

void ABaseAIController::SetupAIController()
{
	if (BehaviorTree && bShouldRun)
	{
		CurrentBlackboard->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTree);
	}
	AIPerceptionComponent->OnPerceptionUpdated.RemoveAll(this);
	AIPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ABaseAIController::OnPerceptionUpdated);
}

AActor* ABaseAIController::GetSeeingPawn()
{
	UObject* object = CurrentBlackboard->GetValueAsObject(BlackboardEnemyKey);
	return object ? Cast<AActor>(object) : nullptr;
}
