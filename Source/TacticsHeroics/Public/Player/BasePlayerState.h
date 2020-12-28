
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "API/Player/BasePlayerStateInterface.h"
#include "BasePlayerState.generated.h"

class UBaseTeam;

UCLASS()
class TACTICSHEROICS_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasePlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const;

	class UBaseAttributeSet* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "BasePlayerState")
		bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "BasePlayerState")
		void ShowAbilityConfirmCancelText(bool ShowText);

	virtual void SetTeam(UBaseTeam* InTeam) { CurrentTeam = InTeam; };
	virtual UBaseTeam* GetTeam() { return CurrentTeam; };

	UFUNCTION(BlueprintCallable, Category = "BasePlayerState")
		virtual EPlayerType GetPlayerType() { return CurrentRole; };
	UFUNCTION(BlueprintCallable, Category = "BasePlayerState")
		virtual void SetPlayerType(EPlayerType InType) { CurrentRole = InType; };

	/**
	* Getters for attributes from GDAttributeSetBase. Returns Current Value unless otherwise specified.
	*/

	UFUNCTION(BlueprintCallable)
		float GetHealth() const { return AttributeSetBase->GetHealth(); } ;
	UFUNCTION(BlueprintCallable)
		void SetHealth(float InHealth) { AttributeSetBase->SetHealth(InHealth); };
	UFUNCTION(BlueprintCallable)
		float GetMaxHealth() const { return AttributeSetBase->GetMaxHealth(); };
	UFUNCTION(BlueprintCallable)
		float GetHealthRegenRate() const { return AttributeSetBase->GetHealthRegenRate(); };
	UFUNCTION(BlueprintCallable)
		float GetStamina() const { return AttributeSetBase->GetStamina(); };
	UFUNCTION(BlueprintCallable)
		virtual void SetStamina(float InStamina) { AttributeSetBase->SetStamina(InStamina); };
	UFUNCTION(BlueprintCallable)
		float GetMaxStamina() const { return AttributeSetBase->GetMaxStamina(); };
	UFUNCTION(BlueprintCallable)
		float GetStaminaRegenRate() const { return AttributeSetBase->GetStaminaRegenRate(); };
	UFUNCTION(BlueprintCallable)
		float GetCurrentArmor() const { return AttributeSetBase->GetCurrentArmor(); };
	UFUNCTION(BlueprintCallable)
		float GetBaseArmor() const { return AttributeSetBase->GetBaseArmor(); };
	UFUNCTION(BlueprintCallable)
		float GetMoveSpeed() const { return AttributeSetBase->GetMoveSpeed(); };
	UFUNCTION(BlueprintCallable)
		float GetBaseMoveSpeed() const { return AttributeSetBase->GetMoveSpeedAttribute().GetGameplayAttributeData(AttributeSetBase)->GetBaseValue(); };

protected:
	UPROPERTY()
		class UBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		class UBaseAttributeSet* AttributeSetBase;

	FGameplayTag DeadTag;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle ManaRegenRateChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void HealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void StaminaRegenRateChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

private:

	UBaseTeam* CurrentTeam;
	EPlayerType CurrentRole;
};
