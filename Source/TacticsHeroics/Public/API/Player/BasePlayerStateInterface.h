#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "API/Game/BaseTeamInterface.h"
#include "BasePlayerStateInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UBasePlayerStateInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBasePlayerStateInterface
{
	GENERATED_BODY()

public:
	/*

	virtual void SetTeam(TScriptInterface<IBaseTeamInterface> InTeam) = 0;
	virtual TScriptInterface<IBaseTeamInterface> GetTeam() = 0;

	virtual EPlayerType GetPlayerType() = 0;
	virtual void SetPlayerType(EPlayerType InType) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetHealth() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void SetHealth(float InHealth) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetMaxHealth() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetHealthRegenRate() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetStamina() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void SetStamina(float InHealth) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetMaxStamina() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetStaminaRegenRate() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetCurrentArmor() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetBaseArmor() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetMoveSpeed() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetBaseMoveSpeed() const = 0;
		*/
};
