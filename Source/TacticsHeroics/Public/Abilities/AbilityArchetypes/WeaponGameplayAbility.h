// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/BaseGameplayAbility.h"
#include "WeaponGameplayAbility.generated.h"

class ABaseWeaponActor;

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API UWeaponGameplayAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData) override;

protected:

};
