// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "API/Abilities/BaseAbilitySystemInterface.h"
#include "BaseAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UBaseAbilitySystemComponent*, SourceAbilityComponent, float, UnmitigatedDamage, float, MitigatedDamage);

class UBaseGameplayAbility;

UCLASS()
class TACTICSHEROICS_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:

	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	FReceivedDamageDelegate ReceivedDamage;

	virtual void ReceiveDamage(UBaseAbilitySystemComponent* SourceAbilityComponent, float UnmitigatedDamage, float MitigatedDamage);

	virtual void GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UBaseGameplayAbility*>& ActiveAbilities);

	static UBaseAbilitySystemComponent* GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = false);

	virtual bool SetCharacterAbilitiesGiven(bool InGiven);
	virtual bool SetStartupEffectsApplied(bool InApplied);

	virtual bool TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation);
	virtual bool GetStartupEffectsApplied();
	virtual void SetTagMapCount(const FGameplayTag& Tag, int32 NewCount);
	virtual TArray<FGameplayAbilitySpec>& GetActivatableAbilities();
	virtual int32 RemoveActiveEffectsWithTags(const FGameplayTagContainer Tags);
	virtual FGameplayEffectSpecHandle MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, FGameplayEffectContextHandle Context) const;
	virtual FGameplayEffectContextHandle MakeEffectContext() const;
	virtual FGameplayAbilitySpecHandle GiveAbility(const FGameplayAbilitySpec& Spec);
	virtual TArray<TPair<float, float>> GetActiveEffectsTimeRemainingAndDuration(const FGameplayEffectQuery& Query) const;
	virtual bool GetCharacterAbilitiesGiven();
	virtual void ClearAbility(const FGameplayAbilitySpecHandle& Handle);
	virtual void CancelAllAbilities();
	virtual void AddLooseGameplayTag(const FGameplayTag& GameplayTag);
};
