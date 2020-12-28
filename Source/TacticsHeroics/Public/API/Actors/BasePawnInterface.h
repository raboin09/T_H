// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Abilities/BaseAttributeSet.h"
#include "UI/Data/Hero/CharacterAbilityWidgetData.h"
#include "BasePawnInterface.generated.h"

class UBaseAbilitySystemComponent;
class UBaseGameplayAbility;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UBasePawnInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBasePawnInterface
{
	GENERATED_BODY()

public:
	/*

	//UFUNCTION(BlueprintCallable)
		virtual float GetHealth() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetMaxHealth() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void OnHealthChanged() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void HandleHealthChange() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetCurrentMoveSpeed() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetBaseMoveSpeed() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetStamina() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual float GetMaxStamina() const = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void SetHealth(float InHealth) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void SetStamina(float InStamina) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual bool IsAlive() = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void RemoveAttachments() = 0;

	//UFUNCTION(BlueprintCallable)
		virtual void SetOwnerTeam(TScriptInterface<IBaseTeamInterface> InTeam) = 0;

	//UFUNCTION(BlueprintCallable)
		virtual TScriptInterface<IBaseTeamInterface> GetOwnerTeam() = 0;

	//UFUNCTION(BlueprintCallable)
		virtual FCharacterCombatAbilitiesWidgetDataStruct GetPawnAbilityInfo() const = 0;

	virtual bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true) = 0;
	virtual bool ActivateAbilityWithClass(TSubclassOf<UBaseGameplayAbility> AbilityClass, bool bAllowRemoteActivation) = 0;
	virtual void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UBaseGameplayAbility*>& ActiveAbilities) = 0;
	virtual bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration) = 0;

	virtual void RemoveAbilities() = 0;
	virtual void Die() = 0;
	virtual void FinishDying() = 0;

	virtual class USpringArmComponent* GetCameraBoom() const = 0;
	virtual class UCameraComponent* GetFollowCamera() const = 0;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const = 0;
	*/
};
