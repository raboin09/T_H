#include "Player/BasePlayerState.h"
#include "GameplayTagContainer.h"
#include "Actors/BaseCharacter.h"

ABasePlayerState::ABasePlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSetBase = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("AttributeSetBase"));

	NetUpdateFrequency = 100.0f;
	DeadTag = FGameplayTag::RequestGameplayTag(FName("State.Dead"));
}

UAbilitySystemComponent * ABasePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UBaseAttributeSet* ABasePlayerState::GetAttributeSetBase() const
{
	return AttributeSetBase;
}

bool ABasePlayerState::IsAlive() const
{
	return GetHealth() > 0.0f;
}

void ABasePlayerState::ShowAbilityConfirmCancelText(bool ShowText)
{
	/*
	AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	if (PC)
	{
		UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->ShowAbilityConfirmCancelText(ShowText);
		}
	}
	*/
}

void ABasePlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &ABasePlayerState::HealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxHealthAttribute()).AddUObject(this, &ABasePlayerState::MaxHealthChanged);
		HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthRegenRateAttribute()).AddUObject(this, &ABasePlayerState::HealthRegenRateChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaAttribute()).AddUObject(this, &ABasePlayerState::StaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetMaxStaminaAttribute()).AddUObject(this, &ABasePlayerState::MaxStaminaChanged);
		StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetStaminaRegenRateAttribute()).AddUObject(this, &ABasePlayerState::StaminaRegenRateChanged);

		AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ABasePlayerState::StunTagChanged);
	}
}

void ABasePlayerState::HealthChanged(const FOnAttributeChangeData & Data)
{
	float Health = Data.NewValue;
	ABaseCharacter* Hero = Cast<ABaseCharacter>(GetPawn());
	/*
	TODO
	if (Hero)
	{
		UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(Health / GetMaxHealth());
		}
	}
	*/

	if (!IsAlive() && !AbilitySystemComponent->HasMatchingGameplayTag(DeadTag))
	{
		if (Hero)
		{
			Hero->Die();
		}
	}
}

void ABasePlayerState::MaxHealthChanged(const FOnAttributeChangeData & Data)
{
	float MaxHealth = Data.NewValue;
	ABaseCharacter* Hero = Cast<ABaseCharacter>(GetPawn());
	/*
	// Update floating status bar

	if (Hero)
	{
		UGDFloatingStatusBarWidget* HeroFloatingStatusBar = Hero->GetFloatingStatusBar();
		if (HeroFloatingStatusBar)
		{
			HeroFloatingStatusBar->SetHealthPercentage(GetHealth() / MaxHealth);
		}
	}

	// Update the HUD
	AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	if (PC)
	{
		UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxHealth(MaxHealth);
		}
	}
	*/
}

void ABasePlayerState::HealthRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float HealthRegenRate = Data.NewValue;
	/*
	// Update the HUD
	AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	if (PC)
	{
		UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetHealthRegenRate(HealthRegenRate);
		}
	}
	*/
}

void ABasePlayerState::StaminaChanged(const FOnAttributeChangeData & Data)
{
	float Stamina = Data.NewValue;
	// Update the HUD
	// Handled in the UI itself using the AsyncTaskAttributeChanged node as an example how to do it in Blueprint
}

void ABasePlayerState::MaxStaminaChanged(const FOnAttributeChangeData & Data)
{
	float MaxStamina = Data.NewValue;
	/*
	// Update the HUD
	AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	if (PC)
	{
		UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetMaxStamina(MaxStamina);
		}
	}
	*/
}

void ABasePlayerState::StaminaRegenRateChanged(const FOnAttributeChangeData & Data)
{
	float StaminaRegenRate = Data.NewValue;
	/*
	// Update the HUD
	AGDPlayerController* PC = Cast<AGDPlayerController>(GetOwner());
	if (PC)
	{
		UGDHUDWidget* HUD = PC->GetHUD();
		if (HUD)
		{
			HUD->SetStaminaRegenRate(StaminaRegenRate);
		}
	}
	*/
}

void ABasePlayerState::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		FGameplayTagContainer AbilityTagsToCancel;
		AbilityTagsToCancel.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));

		FGameplayTagContainer AbilityTagsToIgnore;
		AbilityTagsToIgnore.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.NotCanceledByStun")));

		AbilitySystemComponent->CancelAbilities(&AbilityTagsToCancel, &AbilityTagsToIgnore);
	}
}