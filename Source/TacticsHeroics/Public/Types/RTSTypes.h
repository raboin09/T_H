// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RTSTypes.generated.h"

UENUM(BlueprintType)
enum class EOrderType : uint8 {
	IDLE UMETA(DisplayName = "Idle"),
	MOVE UMETA(DisplayName = "Move"),
	ATTACK UMETA(DisplayName = "Attack"),
	ATTACK_MOVE UMETA(DisplayName = "Attack Move"),
	PATROL UMETA(DisplayName = "Patrol")
};