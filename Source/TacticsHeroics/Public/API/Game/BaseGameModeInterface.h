#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseGameModeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBaseGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseGameModeInterface
{
	GENERATED_BODY()

public:

	// virtual void InitTeams() = 0;

};
