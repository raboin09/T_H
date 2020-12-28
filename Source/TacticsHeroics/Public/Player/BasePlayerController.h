// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "API/Actors/BasePawnInterface.h"
#include "API/Player/BasePlayerStateInterface.h"
#include "API/Player/BasePlayerControllerInterface.h"
#include "BasePlayerController.generated.h"

class ABaseCharacter;
class ABaseGeneralHUD;
class ABasePlayerState;
class UBaseTeam;

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
		
public:

	UFUNCTION(BlueprintCallable)
		virtual void InitHeroPlayer();

	UFUNCTION(BlueprintCallable)
		virtual void InitGeneralPlayer();

	UFUNCTION(BlueprintImplementableEvent, Category = "BasePlayerController")
		void ShowHeroSelectionWidget();

	UFUNCTION(BlueprintNativeEvent, Category = "BasePlayerController")
		void InitHeroMasterWidget();

	UFUNCTION(BlueprintNativeEvent, Category = "BasePlayerController")
		void InitGeneralMasterWidget();

	virtual UBaseTeam* GetOwnerTeam();
	virtual void UnbindInput();
	
	ABaseGeneralHUD* GetGeneralHUD();

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override; 

private:

	void SetupGeneralHUD();


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UUserWidget* InGameCurrentWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ABaseGeneralPawn> GeneralPawnClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class ABaseGeneralHUD* GeneralHUD;

private:
	ABasePlayerState* CurrentPlayerState;
	APawn* CurrentPawn;	



};
 