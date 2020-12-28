// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayAbilityTypes.h"
#include "BaseTargetTypeInterface.generated.h"

class ABaseCharacter;
class AActor;
struct FGameplayEventData;

UINTERFACE(MinimalAPI)
class UBaseTargetTypeInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseTargetTypeInterface
{
	GENERATED_BODY()

public:
    
    // virtual void GetTargets(ABaseCharacter* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const;
    
};
