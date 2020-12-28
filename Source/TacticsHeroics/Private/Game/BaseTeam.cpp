// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseTeam.h"
#include "API/Actors/BasePawnInterface.h"
#include "Actors/Objectives/BaseObjectiveActor.h"
#include "Actors/BaseCharacter.h"
#include "Actors/BaseGeneralPawn.h"
#include "AI/BaseAIController.h"
#include "Types/GameTypes.h"

void UBaseTeam::SetFactionData(FFactionDataStruct InFactionData)
{
	TeamData = InFactionData;
}

FFactionDataStruct UBaseTeam::GetFactionData()
{
	return TeamData;
}

ABaseCharacter* UBaseTeam::SpawnCharacterAtLocation(FVector InSpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseCharacter> CharacterClass, bool bShouldSpawnNewController)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (GetWorld()) {
		ABaseCharacter* SpawnChar = nullptr;
		if (GetNumberOfOwnedObjectives() > 0) {
			InSpawnLocation.X += FMath::RandRange(-200, 200);
			InSpawnLocation.Y += FMath::RandRange(-200, 200);
			InSpawnLocation.Z += 75;
			SpawnChar = GetWorld()->SpawnActor<ABaseCharacter>(CharacterClass, InSpawnLocation, SpawnRotation, SpawnInfo);
			UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - Spawn at objective"));
		}
		if (SpawnChar) {
			if (bShouldSpawnNewController) {
				SpawnChar->SpawnDefaultController();
				ControllerArray.Add(SpawnChar->GetController());
				ABaseAIController* TempAI = Cast<ABaseAIController>(SpawnChar->GetController());
				if (TempAI) {
					TempAI->SetOwnerTeam(this);
				}

			}
			SpawnChar->SetOwnerTeam(this);
			OwnedPawns.Add(SpawnChar);
		}
		return SpawnChar;
	}
	return nullptr;
}

ABaseCharacter* UBaseTeam::SpawnRandomCharacterAtRandomObjective(bool bShouldSpawnNewController)
{
	int32 ObjIndex = FMath::RandRange(0, OwnedObjectives.Num() - 1);
	int32 HeroIndex = FMath::RandRange(0, TeamData.AvailableHeroes.Num() - 1);
	if (OwnedObjectives[ObjIndex] && TeamData.AvailableHeroes[HeroIndex]) {
		return SpawnCharacterAtLocation(OwnedObjectives[ObjIndex]->GetActorLocation(), FRotator::ZeroRotator, TeamData.AvailableHeroes[HeroIndex], bShouldSpawnNewController);
	}
	return nullptr;
}

ABaseGeneralPawn* UBaseTeam::SpawnGeneralAtLocation(FVector SpawnLocation, FRotator SpawnRotation, TSubclassOf<ABaseGeneralPawn> CharacterClass, bool bShouldSpawnNewController)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (GetWorld()) {
		ABaseGeneralPawn* SpawnChar = nullptr;
		if (GetNumberOfOwnedObjectives() > 0) {
			SpawnChar = GetWorld()->SpawnActor<ABaseGeneralPawn>(CharacterClass, SpawnLocation, SpawnRotation, SpawnInfo);
			UE_LOG(LogTemp, Warning, TEXT("TeamComponent::SpawnTeamCharacter - Spawn at objective"));
		}
		if (SpawnChar) {
			if (bShouldSpawnNewController) {
				SpawnChar->SpawnDefaultController();
			}
			SpawnChar->SetOwnerTeam(this);
			OwnedPawns.Add(SpawnChar);
		}
		return SpawnChar;
	}
	return nullptr;
}

ABaseCharacter* UBaseTeam::SpawnCharacterAtActor(AActor* ActorToSpawnAt, TSubclassOf<ABaseCharacter> CharacterClass, bool bShouldSpawnNewController)
{
	return SpawnCharacterAtLocation(ActorToSpawnAt->GetActorLocation(), ActorToSpawnAt->GetActorRotation(), CharacterClass, bShouldSpawnNewController);
}

void UBaseTeam::AddObjectiveToTeam(ABaseObjectiveActor* InObjective)
{
	OwnedObjectives.Add(InObjective);
}

void UBaseTeam::RemoveObjectiveFromTeam(ABaseObjectiveActor* InObjective)
{
	OwnedObjectives.Remove(InObjective);
}

bool UBaseTeam::IsPawnOnThisTeam(AActor* InActor)
{
	APawn* TempPawn = Cast<APawn>(InActor);
	if (OwnedPawns.Contains(TempPawn)) {
		return true;
	}
	return false;
}

bool UBaseTeam::IsPawnOnThisTeam(APawn* InPawn)
{
	if (OwnedPawns.Contains(InPawn)) {
		return true;
	}
	return false;
}

int32 UBaseTeam::GetNumberOfOwnedObjectives()
{
	return OwnedObjectives.Num();
}