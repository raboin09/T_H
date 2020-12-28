// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterAbilityWidgetData.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FCharacterAbilityWidgetDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* AbilityIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText AbilityName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText AbilityDescription;

};

USTRUCT(BlueprintType)
struct FCharacterCombatAbilitiesWidgetDataStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FCharacterAbilityWidgetDataStruct> AbilityWidgetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FCharacterAbilityWidgetDataStruct WeaponWidgetData;

};

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API UCharacterAbilityWidgetData : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "CharacterAbilityData")
		FCharacterCombatAbilitiesWidgetDataStruct GetCombatAbilitiesWidgetDataStruct() const;

private:

	FCharacterCombatAbilitiesWidgetDataStruct CombatAbilitiesStruct;
	
};
