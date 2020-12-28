// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAbilitySystemComponent.h"
#include "API/Abilities/BaseAttributeSetInterface.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TACTICSHEROICS_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();  

	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) ;
	void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) ;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const ;

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
		FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegenRate)
		FGameplayAttributeData HealthRegenRate;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
		FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
		FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina)

	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegenRate)
		FGameplayAttributeData StaminaRegenRate;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, StaminaRegenRate)

	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_BaseArmor)
		FGameplayAttributeData BaseArmor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BaseArmor)

	UPROPERTY(BlueprintReadOnly, Category = "Armor", ReplicatedUsing = OnRep_CurrentArmor)
		FGameplayAttributeData CurrentArmor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CurrentArmor)

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
		FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Mana", meta = (HideFromLevelInfos))
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Damage)

protected:

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);
    
    void AdjustAndClampAttribute(const FGameplayAttribute& Attribute, float& NewValue);
    void AssessAndApplyGameplayEffects(const FGameplayEffectModCallbackData& Data);
    
	UFUNCTION()
		void OnRep_Health();

	UFUNCTION()
		void OnRep_MaxHealth();

	UFUNCTION()
		void OnRep_HealthRegenRate();

	UFUNCTION()
		void OnRep_Stamina();

	UFUNCTION()
		void OnRep_MaxStamina();

	UFUNCTION()
		void OnRep_StaminaRegenRate();

	UFUNCTION()
		void OnRep_BaseArmor();

	UFUNCTION()
		void OnRep_CurrentArmor();

	UFUNCTION()
		void OnRep_MoveSpeed();

};
