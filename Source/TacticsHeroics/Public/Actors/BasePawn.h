// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "API/Actors/BasePawnInterface.h"
#include "API/Abilities/BaseAttributeSetInterface.h"
#include "API/Game/BaseTeamInterface.h"
#include "UI/Data/Hero/CharacterAbilityWidgetData.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "BasePawn.generated.h"

UCLASS()
class TACTICSHEROICS_API ABasePawn : public APawn//, public IBasePawnInterface
{
	GENERATED_BODY()

public:
	ABasePawn();

protected:
	/*
	virtual void BeginPlay() override;

	void AddPawnAbilities();
	void AddStartupEffects();
	void InitializeAttributes();

	void BindAbilityInput();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void TurnAtRate(float Rate);
	void Lookup(float Value);
	void LookUpAtRate(float Rate);

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetHealth() const override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetMaxHealth() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
		void OnHealthChanged() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
		void HandleHealthChange() const override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
		void RemoveAttachments() override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetCurrentMoveSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetBaseMoveSpeed() const override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetStamina() const override;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetMaxStamina() const override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		virtual bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true) override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		virtual void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UBaseGameplayAbility*>& ActiveAbilities) override;

	UFUNCTION(BlueprintCallable, Category = "Abilities")
		virtual bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration) override;

	virtual void RemoveAbilities() override;
	virtual void Die() override;
	virtual bool IsAlive() override;

	virtual void SetHealth(float InHealth) override;
	virtual void SetStamina(float InStamina) override;

	virtual void SetOwnerTeam(TScriptInterface<IBaseTeamInterface> InTeam) override;

	virtual FName GetPawnName() const override;
	virtual FCharacterCombatAbilitiesWidgetDataStruct GetPawnAbilityInfo() const override;

	UFUNCTION(BlueprintCallable, Category = "BasePawn")
		virtual void FinishDying() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class UBaseGameplayAbility>> PawnAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Abilities")
		TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	IBaseAbilitySystemInterface* AbilitySystemComponent;
	IBaseAttributeSetInterface* AttributeBaseSet;
	TScriptInterface<IBaseTeamInterface> OwnerTeam;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(EditDefaultsOnly)
		FName PawnName;

	UPROPERTY(EditDefaultsOnly)
		FCharacterCombatAbilitiesWidgetDataStruct AbilityIconInfo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
		UAnimMontage* DeathMontage;

	bool AbilitiesBoundToInput = false;

public:

	FORCEINLINE virtual class USpringArmComponent* GetCameraBoom() const override { return CameraBoom; }
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const override { return FollowCamera; }
	FORCEINLINE virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return Cast<UAbilitySystemComponent>(AbilitySystemComponent); }
	*/

};
