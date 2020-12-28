#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/GameTypes.h"
#include "API/Actors/BasePawnInterface.h"
#include "BasePlayerControllerInterface.generated.h"

class ABaseCharacter;

UINTERFACE(MinimalAPI, BlueprintType, meta=(CannotImplementInterfaceInBlueprint))
class UBasePlayerControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBasePlayerControllerInterface
{
	GENERATED_BODY()

public:
	/*

	virtual void InitHeroPlayer() = 0;  
	virtual void InitGeneralPlayer() = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TScriptInterface<IBasePawnInterface> GetCurrentPawn() = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TScriptInterface<IBaseTeamInterface> GetOwnerTeam() = 0;

	//UFUNCTION(BlueprintCallable, Category = "BasePlayerController_Interface")
		virtual void InitHeroMasterWidget() = 0;

	//UFUNCTION(BlueprintCallable, Category = "BasePlayerController_Interface")
		virtual void InitGeneralMasterWidget() = 0;
	
	virtual void UnbindInput() = 0;
	*/
};
