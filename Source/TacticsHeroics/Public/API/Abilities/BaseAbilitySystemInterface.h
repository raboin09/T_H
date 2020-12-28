// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemInterface.generated.h"

class UBaseGameplayAbility;

UINTERFACE(MinimalAPI)
class UBaseAbilitySystemInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/*
    virtual void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UBaseGameplayAbility*>& ActiveAbilities) = 0;

	virtual bool SetCharacterAbilitiesGiven(bool InGiven) = 0;
	virtual bool SetStartupEffectsApplied(bool InApplied) = 0;

	virtual bool TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation) = 0;
	virtual bool GetStartupEffectsApplied() = 0;
	virtual void SetTagMapCount(const FGameplayTag& Tag, int32 NewCount) = 0;
	virtual TArray<FGameplayAbilitySpec>& GetActivatableAbilities() = 0;
	virtual int32 RemoveActiveEffectsWithTags(const FGameplayTagContainer Tags) = 0;
	virtual FGameplayEffectSpecHandle MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, FGameplayEffectContextHandle Context) const = 0;
	virtual FGameplayEffectContextHandle MakeEffectContext() const = 0;
	virtual FGameplayAbilitySpecHandle GiveAbility(const FGameplayAbilitySpec& Spec) = 0;
	virtual TArray<TPair<float, float>> GetActiveEffectsTimeRemainingAndDuration(const FGameplayEffectQuery& Query) const = 0;
	virtual bool GetCharacterAbilitiesGiven() = 0;
	virtual void ClearAbility(const FGameplayAbilitySpecHandle& Handle) = 0;
	virtual void CancelAllAbilities() = 0;
	virtual void AddLooseGameplayTag(const FGameplayTag& GameplayTag) = 0;
	*/
};
