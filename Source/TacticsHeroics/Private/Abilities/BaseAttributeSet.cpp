// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "BaseAbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseCharacter.h"
#include "GameplayEffect.h"
#include "API/Player/BasePlayerControllerInterface.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "Player/BasePlayerController.h"

UBaseAttributeSet::UBaseAttributeSet()
{

}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
    AdjustAndClampAttribute(Attribute, NewValue);
}

void UBaseAttributeSet::AdjustAndClampAttribute(const FGameplayAttribute& Attribute, float& NewValue)
{
    if (Attribute == GetMaxHealthAttribute())
    {
        AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
    }
    else if (Attribute == GetMaxStaminaAttribute())
    {
        AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
    }
    else if (Attribute == GetMoveSpeedAttribute())
    {
        NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
    }
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
    AssessAndApplyGameplayEffects(Data);
}

void UBaseAttributeSet::AssessAndApplyGameplayEffects(const FGameplayEffectModCallbackData& Data)
{
    
    FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
    UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;
    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
	ABaseCharacter* TargetCharacter = nullptr;
    if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
    {
        TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
        TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
        TargetCharacter = Cast<ABaseCharacter>(TargetActor);
    }

    AActor* SourceActor = nullptr;
    AController* SourceController = nullptr;
	ABaseCharacter* SourceCharacter = nullptr;
    if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
    {
        SourceActor = Source->AbilityActorInfo->AvatarActor.Get();
        SourceController = Source->AbilityActorInfo->PlayerController.Get();
        if (SourceController == nullptr && SourceActor != nullptr)
        {
            if (APawn* Pawn = Cast<APawn>(SourceActor))
            {
                SourceController = Pawn->GetController();
            }
        }

        if (SourceController)
        {
            SourceCharacter = Cast<ABaseCharacter>(SourceController->GetPawn());
        }
        else
        {
            SourceCharacter = Cast<ABaseCharacter>(SourceActor);
        }

        if (Context.GetEffectCauser())
        {
            SourceActor = Context.GetEffectCauser();
        }
    }
    
    if (Data.EvaluatedData.Attribute == GetDamageAttribute())
    {
        // Try to extract a hit result
        FHitResult HitResult;
        if (Context.GetHitResult())
        {
            HitResult = *Context.GetHitResult();
        }

        // Store a local copy of the amount of damage done and clear the damage attribute
        const float LocalDamageDone = GetDamage();
        SetDamage(0.f);

        if (LocalDamageDone > 0.0f)
        {
            // If character was alive before damage is added, handle damage
            // This prevents damage being added to dead things and replaying death animations
            bool WasAlive = true;

            if (TargetCharacter)
            {
                WasAlive = TargetCharacter->IsAlive();
            }

            if (!TargetCharacter->IsAlive())
            {
                //UE_LOG(LogTemp, Warning, TEXT("%s() %s is NOT alive when receiving damage"), TEXT(__FUNCTION__), *TargetCharacter->GetName());
            }

            // Apply the health change and then clamp it
            const float NewHealth = GetHealth() - LocalDamageDone;
            SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));

            if (TargetCharacter && WasAlive)
            {
                // This is the log statement for damage received. Turned off for live games.
                UE_LOG(LogTemp, Log, TEXT("%s() %s Damage Received: %f"), TEXT(__FUNCTION__), *GetOwningActor()->GetName(), LocalDamageDone);

                // Play HitReact animation and sound with a multicast RPC.
                const FHitResult* Hit = Data.EffectSpec.GetContext().GetHitResult();
				/*
                if (Hit)
                {
                    EGDHitReactDirection HitDirection = TargetCharacter->GetHitReactDirection(Data.EffectSpec.GetContext().GetHitResult()->Location);
                    switch (HitDirection)
                    {
                    case EGDHitReactDirection::Left:
                        TargetCharacter->PlayHitReact(HitDirectionLeftTag, SourceCharacter);
                        break;
                    case EGDHitReactDirection::Front:
                        TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
                        break;
                    case EGDHitReactDirection::Right:
                        TargetCharacter->PlayHitReact(HitDirectionRightTag, SourceCharacter);
                        break;
                    case EGDHitReactDirection::Back:
                        TargetCharacter->PlayHitReact(HitDirectionBackTag, SourceCharacter);
                        break;
                    }
                }
                else
                {
                    // No hit result. Default to front.
                    TargetCharacter->PlayHitReact(HitDirectionFrontTag, SourceCharacter);
                }
				*/

                // Show damage number for the Source player unless it was self damage
				/*
                if (SourceActor != TargetActor)
                {
                    IBasePlayerControllerInterface* CurrController = Cast<ABasePlayerController>(SourceController);
                    if (CurrController)
                    {
						CurrController->ShowDamageNumber(LocalDamageDone, TargetCharacter);
                    }
                }
				*/

				/*
                if (!TargetCharacter->IsAlive())
                {
                    // TargetCharacter was alive before this damage and now is not alive, give XP and Gold bounties to Source.
                    // Don't give bounty to self.
                    if (SourceController != TargetController)
                    {
                        // Create a dynamic instant Gameplay Effect to give the bounties
                        UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
                        GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

                        int32 Idx = GEBounty->Modifiers.Num();
                        GEBounty->Modifiers.SetNum(Idx + 2);

                        FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
                        InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
                        InfoXP.ModifierOp = EGameplayModOp::Additive;
                        InfoXP.Attribute = UGDAttributeSetBase::GetXPAttribute();

                        FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
                        InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
                        InfoGold.ModifierOp = EGameplayModOp::Additive;
                        InfoGold.Attribute = UGDAttributeSetBase::GetGoldAttribute();

                        Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
                    }
                }
				*/
            }
        }
    }
    
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
		if (TargetCharacter) {
			TargetCharacter->PlayReactHitVoice();
		}
        SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
    }
    else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
    {
        SetStamina(FMath::Clamp(GetStamina(), 0.0f, GetMaxStamina()));
    }
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, StaminaRegenRate, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BaseArmor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty)
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilityComp->ApplyModToAttributeUnsafe(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UBaseAttributeSet::OnRep_Health()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Health);
}

void UBaseAttributeSet::OnRep_MaxHealth()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth);
}

void UBaseAttributeSet::OnRep_HealthRegenRate()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegenRate);
}

void UBaseAttributeSet::OnRep_Stamina()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Stamina);
}

void UBaseAttributeSet::OnRep_MaxStamina()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxStamina);
}

void UBaseAttributeSet::OnRep_StaminaRegenRate()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, StaminaRegenRate);
}

void UBaseAttributeSet::OnRep_BaseArmor()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BaseArmor);
}

void UBaseAttributeSet::OnRep_CurrentArmor()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentArmor);
}

void UBaseAttributeSet::OnRep_MoveSpeed()
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MoveSpeed);
}