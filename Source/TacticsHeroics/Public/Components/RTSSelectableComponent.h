// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RTSSelectableComponent.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
class USoundCue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentSelectedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRTSSelectableComponentDeselectedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TACTICSHEROICS_API URTSSelectableComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
		UMaterialInterface* SelectionCircleMaterial;

	/** Sound to play when the actor is selected. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
		USoundCue* SelectedSound;


	virtual void BeginPlay() override;


	/** Selects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
		void SelectActor();

	/** Deselects the unit for the local player. */
	UFUNCTION(BlueprintCallable)
		void DeselectActor();

	/** Checks whether the unit is currently selected by the local player, or not. */
	UFUNCTION(BlueprintCallable)
		bool IsSelected();


	/** Event when the actor has been deselected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
		FRTSSelectableComponentDeselectedSignature OnDeselected;

	/** Event when the actor has been selected. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
		FRTSSelectableComponentSelectedSignature OnSelected;


private:
	/** Whether the unit is currently selected by the local player, or not. */
	bool bSelected;

	/** Decal used for rendering the selection circle of this character. */
	UDecalComponent* DecalComponent;

	/** Material instance for rendering the selection circle of the actor. */
	UMaterialInstanceDynamic* SelectionCircleMaterialInstance;
		
};
