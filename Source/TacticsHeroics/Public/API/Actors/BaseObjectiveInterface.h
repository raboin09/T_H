#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseObjectiveInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UBaseObjectiveInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseObjectiveInterface
{
	GENERATED_BODY()

public:
	/*
	UFUNCTION(BlueprintCallable, Category = "BaseObjective")
		virtual void SetOwnerTeam(TScriptInterface<IBaseTeamInterface> InTeam) = 0;
	UFUNCTION(BlueprintCallable, Category = "BaseObjective")
		virtual TScriptInterface<IBaseTeamInterface> GetOwnerTeam() const = 0;
	UFUNCTION(BlueprintCallable, Category = "BaseObjective")
		virtual FVector GetActorLocationOverride() const = 0;
		*/
};
