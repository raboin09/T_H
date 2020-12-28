// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilityTypes.h"
#include "Abilities/BaseGameplayAbility.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InGameUtils.generated.h"

class USoundCue;

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API UInGameUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "HitResults,TargetActors"))
		static FBaseGameplayEffectContainerSpec AddTargetsToEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors);

	UFUNCTION(BlueprintCallable, Category = Ability)
		static TArray<FActiveGameplayEffectHandle> ApplyExternalEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec);

	UFUNCTION(BlueprintCallable, Category = Ability)
		static USoundCue* PickRandomSoundInArray(TArray<USoundCue*> InArray);
};
