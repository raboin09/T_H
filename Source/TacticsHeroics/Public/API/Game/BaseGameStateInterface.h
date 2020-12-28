#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "API/Game/BaseTeamInterface.h"
#include "BaseGameStateInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBaseGameStateInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseGameStateInterface
{
	GENERATED_BODY()

public:

	virtual void AddTeam(IBaseTeamInterface* InTeam) = 0;
	virtual IBaseTeamInterface* GetTeamFromID(int32 InIndex) = 0;
	virtual IBaseTeamInterface* FindTeamOfPlayer(int32 PlayerID) = 0;

	virtual TArray<IBaseObjectiveInterface*> GetAllObjectives() = 0;
	virtual void ObjectiveCaptured(IBaseTeamInterface* CapturingTeam, IBaseObjectiveInterface* CapturedObjective) = 0;
};
