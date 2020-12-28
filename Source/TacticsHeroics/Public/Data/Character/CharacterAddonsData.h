// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "Types/GameTypes.h"
#include "CharacterAddonsData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TACTICSHEROICS_API UCharacterAddonsData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterAddonStruct AddonStruct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EFactionNames DefaultFaction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName CharacterName;
};
