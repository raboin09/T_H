#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Types/GameTypes.h"
#include "Data/UI/InGame/HeroWidgetData.h"
#include "Data/Game/FactionData.h"
#include "BaseGameInstanceInterface.generated.h"

UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UBaseGameInstanceInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseGameInstanceInterface
{
	GENERATED_BODY()

public:
	/*
	//UFUNCTION(BlueprintCallable)
		virtual void SetPersistentID(int32 InID) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual int32 GetPersistentID() const = 0;
	
	//UFUNCTION(BlueprintCallable)
		virtual void SetTeamData(TMap<int32, FMainMenuTeamStruct> InMap) = 0;
	
	//UFUNCTION(BlueprintCallable)
		virtual TMap<int32, FMainMenuTeamStruct> GetTeamData() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TArray<UFactionData*> GetFactionData() = 0;
		*/
};
