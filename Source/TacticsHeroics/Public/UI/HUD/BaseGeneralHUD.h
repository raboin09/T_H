// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Actors/BaseCharacter.h"
#include "BaseGeneralHUD.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API ABaseGeneralHUD : public AHUD
{
	GENERATED_BODY()

public:

	FVector2D Get2DMousePosition();
	virtual void DrawHUD();

	FVector2D InitialPoint;
	FVector2D CurrentPoint;
	bool bStartSelecting = false;
	TArray<ABaseCharacter*> FoundActors;
};
