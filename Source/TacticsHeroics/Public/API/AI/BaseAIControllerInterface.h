#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BaseAIControllerInterface.generated.h"

UENUM(BlueprintType)
enum class EAIStatus : uint8 {
	ACTIVE UMETA(DisplayName = "Active"),
	INACTIVE UMETA(DisplayName = "Inactive"),
	COMBAT UMETA(DisplayName = "Combat"),
	UNPOSSESSED UMETA(DisplayName = "Unpossessed")
};

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UBaseAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseAIControllerInterface
{
	GENERATED_BODY()

public:
	/*
	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual void HandlePawnDeath() = 0;

	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual void SpawnNewPawn() = 0;

	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual TScriptInterface<IBasePawnInterface> GetPossessedPawn() = 0;

	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual void SetupAIController() = 0;

	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors) = 0;

	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual AActor* GetSeeingPawn() = 0;

	UFUNCTION(BlueprintCallable, Category = "BaseAIController")
		virtual void ChangePawnState(EAIStatus NewStatus) = 0;
		*/
};
