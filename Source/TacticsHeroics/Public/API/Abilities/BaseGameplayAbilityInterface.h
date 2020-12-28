// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffect.h"
#include "GameplayAbilitySpec.h"
#include "BaseGameplayAbilityInterface.generated.h"

UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UBaseGameplayAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseGameplayAbilityInterface
{
	GENERATED_BODY()

public:
	/*
         UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
             virtual FBaseGameplayEffectContainerSpec MakeEffectContainerSpecFromContainer(const FBaseGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1) = 0;

         UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
             virtual FBaseGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1) = 0;

         UFUNCTION(BlueprintCallable, Category = Ability)
             virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec) = 0;

         UFUNCTION(BlueprintCallable, Category = Ability, meta = (AutoCreateRefTerm = "EventData"))
             virtual TArray<FActiveGameplayEffectHandle> ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1) = 0;

			 */
};
