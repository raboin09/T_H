// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayAbilityTypes.h"
#include "BaseTargetType.generated.h"

class ABaseCharacter;
class AActor;
struct FGameplayEventData;

UENUM(BlueprintType)
enum class EValidTargetType : uint8 {
	ALLIES UMETA(DisplayName = "Allies"),
	ENEMIES UMETA(DisplayName = "Enemies"),
	ALL UMETA(DisplayName = "All")
};

UCLASS(Blueprintable, meta = (ShowWorldContextPin))
class TACTICSHEROICS_API UBaseTargetType : public UObject
{
	GENERATED_BODY()

public:
	UBaseTargetType() {}

	UFUNCTION(BlueprintNativeEvent)
		void GetTargets(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;

protected:

	UFUNCTION(BlueprintCallable)
		bool IsValidTarget(APawn* TargetingCharacter, const AActor* TargetingActor, EValidTargetType InType) const;

};

UCLASS(NotBlueprintable)
class TACTICSHEROICS_API UBaseTargetType_UseOwner : public UBaseTargetType
{
	GENERATED_BODY()

public:
	UBaseTargetType_UseOwner() {}

	virtual void GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

UCLASS(NotBlueprintable)
class TACTICSHEROICS_API UBaseTargetType_UseEventData_ALL : public UBaseTargetType
{
	GENERATED_BODY()

public:
	UBaseTargetType_UseEventData_ALL() {}

	virtual void GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

UCLASS(NotBlueprintable)
class TACTICSHEROICS_API UBaseTargetType_UseEventData_ALLIES : public UBaseTargetType
{
	GENERATED_BODY()

public:
	UBaseTargetType_UseEventData_ALLIES() {}

	virtual void GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};


UCLASS(NotBlueprintable)
class TACTICSHEROICS_API UBaseTargetType_UseEventData_ENEMIES : public UBaseTargetType
{
	GENERATED_BODY()

public:
	UBaseTargetType_UseEventData_ENEMIES() {}

	virtual void GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};


UCLASS(NotBlueprintable)
class TACTICSHEROICS_API UBaseTargetType_UseEventData_SELF : public UBaseTargetType
{
	GENERATED_BODY()

public:
	UBaseTargetType_UseEventData_SELF() {}

	virtual void GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;
};

UCLASS(Blueprintable, Abstract)
class TACTICSHEROICS_API UBaseTargetType_ArcHitbox : public UBaseTargetType
{
	GENERATED_BODY()

public:
	UBaseTargetType_ArcHitbox() {}

	virtual void GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const override;

protected:

	UPROPERTY(EditDefaultsOnly)
		EValidTargetType ValidTarget;
	UPROPERTY(EditDefaultsOnly)
		FVector Offset;
	UPROPERTY(EditDefaultsOnly)
		float NumCones;
	UPROPERTY(EditDefaultsOnly)
		float ConeTraceArcWidth;
	UPROPERTY(EditDefaultsOnly)
		float SweepRadius;		
	UPROPERTY(EditDefaultsOnly)
		float ArcDistance;
	UPROPERTY(EditDefaultsOnly)
		bool bShowDebug;
};
