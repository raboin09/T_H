
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttributeSet.h"
#include "BaseAbilitySystemComponent.h"
#include "BaseAttributeSetInterface.generated.h"

UINTERFACE(MinimalAPI)
class UBaseAttributeSetInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseAttributeSetInterface
{
	GENERATED_BODY()

public:
	/*
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) = 0;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) = 0;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const = 0;

	virtual float GetHealth() const = 0;
	virtual void SetHealth(float NewVal) const = 0;

	virtual float GetHealthRegenRate() const = 0;
	virtual void SetHealthRegenRate(float NewVal) const = 0;

	virtual float GetMaxHealth() const = 0;
	virtual void SetMaxHealth(float NewVal) const = 0;

	virtual float GetStamina() const = 0;
	virtual void SetStamina(float NewVal) const = 0;

	virtual float GetMaxStamina() const = 0;
	virtual void SetMaxStamina(float NewVal) const = 0;

	virtual float GetStaminaRegenRate() const = 0;
	virtual void SetStaminaRegenRate(float NewVal) const = 0;

	virtual float GetBaseArmor() const = 0;
	virtual void SetBaseArmor(float NewVal) const = 0;

	virtual float GetCurrentArmor() const = 0;
	virtual void SetCurrentArmor(float NewVal) const = 0;

	virtual float GetMoveSpeed() const = 0;
	virtual FGameplayAttributeData GetMoveSpeedAttributeData() const = 0;
	virtual void SetMoveSpeed(float NewVal) const = 0;
	*/
};
