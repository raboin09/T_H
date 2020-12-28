// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Abilities/BaseAttributeSet.h"
#include "API/Actors/BasePawnInterface.h"
#include "Types/GameTypes.h"
#include "API/Actors/BaseCharacterInterface.h"
#include "API/Game/BaseTeamInterface.h"
#include "UI/Data/Hero/CharacterAbilityWidgetData.h"
#include "API/Player/BasePlayerStateInterface.h"
#include "API/Abilities/BaseAttributeSetInterface.h"
#include "Player/BasePlayerController.h"
#include "Types/LocomotionAnimTypes.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "BaseCharacter.generated.h"

class UBaseAbilitySystemComponent;
class UBaseGameplayAbility;
class ABasePlayerState;
class UBaseTeam;
class UDecalComponent;
class ABaseWeaponActor;
class UCharacterAddonsData;
class URTSSelectableComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDiedDelegate, ABaseCharacter*, Character);

UCLASS(Abstract)
class TACTICSHEROICS_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();

	UPROPERTY(BlueprintAssignable, Category = BaseCharacter)
		FCharacterDiedDelegate OnCharacterDied;

	UFUNCTION(BlueprintCallable, Category = "Input Component")
		virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void Tick(float DeltaTime) override;

	UBaseAttributeSet* GetAttributes() { return AttributeSetBase; };

		float GetHealth() const;
		float GetMaxHealth() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
		void OnHealthChanged() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Attributes")
		void HandleHealthChange() const;

	UFUNCTION(BlueprintImplementableEvent, Category = "Character")
		void RemoveAttachments();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void PlayEntranceVoice();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void PlayAbilityUseVoice();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void PlayDeathVoice();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void PlayWeaponAttackVoice();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Character")
		void PlayReactHitVoice();

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetCurrentMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetBaseMoveSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetStamina() const;

	UFUNCTION(BlueprintCallable, Category = "Attributes")
		virtual float GetMaxStamina() const;

	UFUNCTION(BlueprintCallable, Category = Abilities)
		virtual bool ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true);

	UFUNCTION(BlueprintCallable, Category = Abilities)
		virtual bool ActivateAttackWithClass(TSubclassOf<UBaseGameplayAbility> AbilityClass, bool bAllowRemoteActivation);

	UFUNCTION(BlueprintCallable, Category = Abilities)
		virtual bool ForceActivateAbilityWithClass(TSubclassOf<UBaseGameplayAbility> AbilityClass, bool bAllowRemoteActivation);

	UFUNCTION(BlueprintCallable, Category = Abilities)
		virtual void GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UBaseGameplayAbility*>& ActiveAbilities);

	UFUNCTION(BlueprintCallable, Category = Abilities)
		virtual bool GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration);

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
		virtual void Die();

	UFUNCTION(BlueprintCallable)
		virtual void SetHealth(float InHealth);

	UFUNCTION(BlueprintCallable)
		virtual void SetStamina(float InStamina);

	UFUNCTION(BlueprintCallable)
		virtual bool IsAlive();

	virtual void RemoveAbilities();

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
		void SetCurrentWeapon(ABaseWeaponActor* InActor) { CurrentWeapon = InActor; };

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
		ABaseWeaponActor* GetCurrentWeapon() { return CurrentWeapon; };

	FName GetWeaponAttachPoint(int32 InInt) { return Addons.Weapons[InInt].WeaponAttachPoint; };

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
		virtual void SetOwnerTeam(UBaseTeam* InTeam) { OwnerTeam = InTeam; };
	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
		virtual UBaseTeam* GetOwnerTeam() { return OwnerTeam; };
	UFUNCTION(BlueprintCallable)
		virtual FCharacterCombatAbilitiesWidgetDataStruct GetPawnAbilityInfo() const;

	UFUNCTION(BlueprintCallable, Category = BaseCharacter)
		virtual void FinishDying();

	UFUNCTION(Server, unreliable, WithValidation)
		void Server_FinishDying();

	UFUNCTION(BlueprintCallable)
		bool CanPlayVoice() { return bCanPlayVoices; };

	bool CanAttack() { return bCanAttack; };

	// ANIM RELATED FUNCTIONS 

	float GetTurnLeanAmount() { return TurnLeanAmount; };


protected:

	UFUNCTION(BlueprintCallable)
		void BlockVoicesForDuration(USoundCue* InSoundCue);

	virtual void BeginPlay() override;

	void AddCharacterAbilities();
	void AddStartupEffects();
	void InitializeAttributes();

	void BindAbilityInput();
    
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Turn(float Value);
    void Lookup(float Value);

	void InitWeaponActor();

	virtual void OnRep_PlayerState() override;

	virtual void SetupAbilitySystem() PURE_VIRTUAL(ABaseCharacter::SetupAbilitySystem, return;);

protected:

	bool bIsAttacking;

	UPROPERTY(EditDefaultsOnly, Category = BaseCharacter)
		UCharacterAddonsData* AddonDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BaseCharacter)
		UAnimMontage* DeathMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = BaseCharacter)
		bool bCanAttack;

	UPROPERTY(BlueprintReadOnly, Category = BaseCharacter)
		FCharacterAddonStruct Addons;
		
	UPROPERTY(BlueprintReadOnly, Category = BaseCharacter)
		EFactionNames DefaultFaction;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	UBaseAbilitySystemComponent* AbilitySystemComponent;
	ABasePlayerState* CurrentPlayerState;
	UBaseAttributeSet* AttributeSetBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		URTSSelectableComponent* SelectComponent;

	UPROPERTY(BlueprintReadOnly)
		ABaseWeaponActor* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly)
		FCharacterCombatAbilitiesWidgetDataStruct AbilityIconInfo;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UBaseTeam* OwnerTeam;

	bool AbilitiesBoundToInput = false;

private:

	bool bCanPlayVoices;

	float TurnLeanAmount;

public:
	FORCEINLINE virtual class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE virtual class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override { return Cast<UAbilitySystemComponent>(AbilitySystemComponent); }
};
