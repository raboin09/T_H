// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CharacterSelectionWidgetData.generated.h"

class ABaseCharacter;

USTRUCT(BlueprintType)
struct FHeroSelectionCard
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FText HeroName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<ABaseCharacter> HeroClass;

};



USTRUCT(BlueprintType)
struct FCharacterSelectionWidgetDataStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FHeroSelectionCard> HeroCards;

};

UCLASS()
class TACTICSHEROICS_API UCharacterSelectionWidgetData : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "CharacterSelectData")
		FCharacterSelectionWidgetDataStruct GetCharacterSelectionWidgetDataStruct() const;

private:

	FCharacterSelectionWidgetDataStruct CharacterSelectionDataStruct;
	
};
