// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameMode.h"
#include "GameTypes.generated.h"

class ABaseCharacter;
class USkeletalMesh;
class ABaseWeaponActor;
class UBaseGameplayAbility;
class USoundCue;

USTRUCT(BlueprintType)
struct FCharacterVoicesStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<USoundCue*> EntranceVoices;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<USoundCue*> AbilityUseVoices;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<USoundCue*> DeathVoices;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<USoundCue*> WeaponUseVoices;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<USoundCue*> ReactHitVoices;
};

USTRUCT(BlueprintType)
struct FCharacterWeaponStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<ABaseWeaponActor> WeaponClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		FName WeaponAttachPoint;

};


USTRUCT(BlueprintType)
struct FCharacterAddonStruct : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<UBaseGameplayAbility> Weapon_1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<UBaseGameplayAbility> Trinket_1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<UBaseGameplayAbility> Ability_1;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<UBaseGameplayAbility> Ability_2;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<UBaseGameplayAbility> Ability_3;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<UBaseGameplayAbility> Ability_4;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		TArray<FCharacterWeaponStruct> Weapons;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Abilities)
		FCharacterVoicesStruct Voices;

};

UENUM(BlueprintType)
enum class EFactionNames : uint8 {
	DWARVES UMETA(DisplayName = "Dwarves"),
	UNDEAD UMETA(DisplayName = "Undead"),
	NEUTRAL UMETA(DisplayName = "Neutral")
};

UENUM(BlueprintType)
enum class EPlayerType : uint8 {
	HERO UMETA(DisplayName = "Hero"),
	GENERAL UMETA(DisplayName = "General")
};

USTRUCT(BlueprintType)
struct FFactionDataStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<ABaseCharacter>> AvailableHeroes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName FactionName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* FactionFlagMesh;
};

USTRUCT(BlueprintType)
struct FMainMenuPlayerStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EPlayerType SelectedRole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PlayerID;
};

USTRUCT(BlueprintType)
struct FMainMenuTeamStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FFactionDataStruct SelectedFactionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<int32, FMainMenuPlayerStruct> CurrentPlayerData;

	FMainMenuTeamStruct() : SelectedFactionData(), CurrentPlayerData()
	{}
};

USTRUCT(BlueprintType)
struct FLevelDataStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSoftObjectPtr<UWorld> Map;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LevelName;
};

USTRUCT(BlueprintType)
struct FMainMenuGameStruct
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, EPlayerType> AvailableRoles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FFactionDataStruct> AvailableFactions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<FString, FLevelDataStruct> AvailableLevels;

};