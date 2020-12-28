// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Input/Events.h"
#include "BaseGeneralPawn.generated.h"

class UBaseTeam;
class ABasePlayerController;
class ABaseCharacter;
class UBaseGeneralPawnMovementComponent;

UCLASS()
class TACTICSHEROICS_API ABaseGeneralPawn : public APawn
{
	GENERATED_BODY()

public:
	ABaseGeneralPawn();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetOwnerTeam(UBaseTeam* InTeam);

	virtual UPawnMovementComponent* GetMovementComponent() const override;
	void SetPlayerOwner(ABasePlayerController* InController);

	void PossessedBy(AController* NewController) override;

	// RTS Functions
public:
	UFUNCTION(BlueprintCallable)
		void SelectActors(TArray<AActor*> Actors);

	UFUNCTION(BlueprintCallable)
		TArray<AActor*> GetSelectedActors();

	virtual void NotifyOnActorOwnerChanged(AActor* Actor);
	virtual void NotifyOnErrorOccurred(const FString& ErrorMessage);
	virtual void NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);
	virtual void NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);
	virtual void NotifyOnIssuedStopOrder(APawn* OrderedPawn);
	virtual void NotifyOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition);
	virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

	UFUNCTION(BlueprintCallable)
		AActor* GetHoveredActor();

	bool GetSelectionFrame(FIntRect& OutSelectionFrame);

	// TODO Make these getters and setters

	/** Mouse position on screen when creating the selection frame started. */
	FVector2D SelectionFrameMouseStartPosition;

	/** Whether we're currently creating a selection frame by dragging the mouse. */
	bool bCreatingSelectionFrame;

	/** Whether the hotkey for showing all health bars is currently pressed, or not. */
	bool bHealthBarHotkeyPressed;

	/** Whether to add clicked units to the current selection. */
	bool bAddSelectionHotkeyPressed;

	/** Whether to add clicked units to the current selection, if they're not already selected, and deselect them otherwise. */
	bool bToggleSelectionHotkeyPressed;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartSelectActors();
	void FinishSelectActors();
	void StartAddSelection();
	void StopAddSelection();
	void StartToggleSelection();
	void StopToggleSelection();

	UFUNCTION()
		void IssueOrder();

	UFUNCTION(BlueprintCallable)
		void IssueStopOrder();

	UFUNCTION(BlueprintCallable)
		bool IssueAttackOrder(AActor* Target);

	UFUNCTION(BlueprintCallable)
		bool IssueMoveOrder(const FVector& TargetLocation);

	void IssueOrderTargetingObjects(TArray<FHitResult>& HitResults);

private:

	bool GetObjectsInSelectionFrame(TArray<FHitResult>& OutHitResults);
	bool IsSelectableActor(AActor* Actor);
	bool GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults);
	bool GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults);
	bool TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults);

	UFUNCTION(Reliable, Server, WithValidation)
		void ServerIssueAttackOrder(APawn* OrderedPawn, AActor* Target);

	/** Orders the passed unit to move to the specified location. */
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerIssueMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Orders the passed unit to stop all current actions. */
	UFUNCTION(Reliable, Server, WithValidation)
		void ServerIssueStopOrder(APawn* OrderedPawn);

protected:

private:
	UBaseGeneralPawnMovementComponent* PawnMovementComponent;
	UBaseTeam* OwnerTeam;
	ABasePlayerController* Owner;

	// RTS Variables

	/** Saved selections of this player. */
	TArray<TArray<AActor*>> ControlGroups;

	/** Actor currently hovered by this player. */
	AActor* HoveredActor;

	/** World position currently hovered by this player. */
	FVector HoveredWorldPosition;

	/** Actors selected by this player. */
	TArray<AActor*> SelectedActors;

	/** Type of the building currently being placed, if any. */
	TSubclassOf<AActor> BuildingBeingPlacedClass;

	/** Time to wait before playing the next selection sound, in seconds. */
	float SelectionSoundCooldownRemaining;
};
