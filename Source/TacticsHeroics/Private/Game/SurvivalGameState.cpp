#include "SurvivalGameState.h"
#include "Game/BaseTeam.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/Objectives/BaseObjectiveActor.h"
#include "API/Game/BaseGameInstanceInterface.h"
#include "Game/BaseGameInstance.h"
#include "UnrealNetwork.h"

ASurvivalGameState::ASurvivalGameState()
{

}

void ASurvivalGameState::BeginPlay()
{

}

void ASurvivalGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASurvivalGameState, ActiveObjectives);
	DOREPLIFETIME(ASurvivalGameState, PlayerTeam);
	DOREPLIFETIME(ASurvivalGameState, AITeam);
}

void ASurvivalGameState::InitTeams()
{
	InitPlayerTeam();
	InitAITeam();
}

void ASurvivalGameState::SpawnAI()
{
	TArray<ABaseObjectiveActor*> ObjsToSpawnAt = FindAllObjectivesForTeam(AITeam);
	for (ABaseObjectiveActor* CurrObj : ObjsToSpawnAt) {
		UBaseGameInstance* GameInst = Cast<UBaseGameInstance>(GetGameInstance());
		if (GameInst && AITeam) {
			for (uint32 i = 0; i < GameInst->NumSkeletonsToSpawn; i++) {
				AITeam->SpawnCharacterAtLocation(CurrObj->GetActorLocation(), FRotator::ZeroRotator, GameInst->AICharClass, true);
			}
		}
	}

	ObjsToSpawnAt = FindAllObjectivesForTeam(PlayerTeam);
	for (ABaseObjectiveActor* CurrObj : ObjsToSpawnAt) {
		UBaseGameInstance* GameInst = Cast<UBaseGameInstance>(GetGameInstance());
		if (GameInst && PlayerTeam && ObjsToSpawnAt[0]) {
			for (uint32 i = 0; i < GameInst->NumDwarvesToSpawn; i++) {
				uint32 CharClassInt = UKismetMathLibrary::RandomInteger(PlayerTeam->GetTeamData().AvailableHeroes.Num());
				TSubclassOf<ABaseCharacter> CharClass = PlayerTeam->GetTeamData().AvailableHeroes[CharClassInt];
				PlayerTeam->SpawnCharacterAtLocation(CurrObj->GetActorLocation(), FRotator::ZeroRotator, CharClass, true);
			}		
		}
	}
}

void ASurvivalGameState::InitPlayerTeam()
{
	UBaseTeam* TempTeam = NewObject<UBaseTeam>(UBaseTeam::StaticClass());
	PlayerTeam = TempTeam;
	PlayerTeam->SetWorld(GetWorld());
	if (PlayerTeam) {
		UBaseGameInstance* GameInst = Cast<UBaseGameInstance>(GetGameInstance());
		if (GameInst) {
			UFactionData* DwarfFaction = GameInst->GetFactionData()[0];
			if (DwarfFaction) {
				PlayerTeam->SetFactionData(DwarfFaction->GetFactionData());
			}
		}
	}
}

void ASurvivalGameState::InitAITeam()
{
	UBaseTeam* TempTeam = NewObject<UBaseTeam>(UBaseTeam::StaticClass());
	AITeam = TempTeam;
	AITeam->SetWorld(GetWorld());
	UBaseGameInstance* GameInst = Cast<UBaseGameInstance>(GetGameInstance());
	if (GameInst) {
		UFactionData* UndeadFaction = GameInst->GetFactionData()[1];
		if (UndeadFaction) {
			AITeam->SetFactionData(UndeadFaction->GetFactionData());
		}
	}
}

TArray<ABaseObjectiveActor*> ASurvivalGameState::FindAllObjectivesForTeam(UBaseTeam* InTeam)
{
	TArray<AActor*> FoundActors;
	TArray<ABaseObjectiveActor*> ReturnObjs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseObjectiveActor::StaticClass(), FoundActors);
	TArray<ABaseObjectiveActor*> FoundObjectives;
	for (AActor* CurrActor : FoundActors) {
		ABaseObjectiveActor* FoundActor = Cast<ABaseObjectiveActor>(CurrActor);
		if(FoundActor) {
			FoundObjectives.Add(FoundActor);
		}
	}

	for (ABaseObjectiveActor* CurrObj : FoundObjectives) {
		if (CurrObj->GetOwnerTeam() == InTeam) {
			ReturnObjs.Add(CurrObj);
		}
	}

	return ReturnObjs;
}
