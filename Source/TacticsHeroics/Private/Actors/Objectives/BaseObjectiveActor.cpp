#include "Actors/Objectives/BaseObjectiveActor.h"
#include "API/Game/BaseTeamInterface.h"

ABaseObjectiveActor::ABaseObjectiveActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ObjectiveMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
}

void ABaseObjectiveActor::BeginPlay()
{
	Super::BeginPlay();	
}
