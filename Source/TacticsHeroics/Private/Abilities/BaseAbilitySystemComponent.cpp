#include "BaseAbilitySystemComponent.h"
#include "Abilities/BaseGameplayAbility.h"
#include "AbilitySystemGlobals.h"

void UBaseAbilitySystemComponent::ReceiveDamage(UBaseAbilitySystemComponent* SourceAbilityComponent, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceAbilityComponent, UnmitigatedDamage, MitigatedDamage);
}

void UBaseAbilitySystemComponent::GetActiveAbilitiesWithTags(const FGameplayTagContainer& GameplayTagContainer, TArray<UBaseGameplayAbility*>& ActiveAbilities)
{
	TArray<FGameplayAbilitySpec*> AbilitiesToActivate;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(GameplayTagContainer, AbilitiesToActivate, false);

	for (FGameplayAbilitySpec* Spec : AbilitiesToActivate)
	{
		TArray<UGameplayAbility*> AbilityInstances = Spec->GetAbilityInstances();

		for (UGameplayAbility* ActiveAbility : AbilityInstances)
		{
			ActiveAbilities.Add(Cast<UBaseGameplayAbility>(ActiveAbility));
		}
	}
}

UBaseAbilitySystemComponent* UBaseAbilitySystemComponent::GetAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= false*/)
{
	return Cast<UBaseAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

bool UBaseAbilitySystemComponent::SetCharacterAbilitiesGiven(bool InGiven)
{
	CharacterAbilitiesGiven = InGiven;
	return CharacterAbilitiesGiven;
}

bool UBaseAbilitySystemComponent::SetStartupEffectsApplied(bool InApplied)
{
	StartupEffectsApplied = InApplied;
	return StartupEffectsApplied;
}

bool UBaseAbilitySystemComponent::TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation)
{
	return Super::TryActivateAbilitiesByTag(GameplayTagContainer, bAllowRemoteActivation);
}

bool UBaseAbilitySystemComponent::GetStartupEffectsApplied()
{
	return StartupEffectsApplied;
}

void UBaseAbilitySystemComponent::SetTagMapCount(const FGameplayTag& Tag, int32 NewCount)
{
	Super::SetTagMapCount(Tag, NewCount);
}

TArray<FGameplayAbilitySpec>& UBaseAbilitySystemComponent::GetActivatableAbilities()
{
	return Super::GetActivatableAbilities();
}

int32 UBaseAbilitySystemComponent::RemoveActiveEffectsWithTags(const FGameplayTagContainer Tags)
{
	return Super::RemoveActiveEffectsWithTags(Tags);
}

FGameplayEffectSpecHandle UBaseAbilitySystemComponent::MakeOutgoingSpec(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, FGameplayEffectContextHandle Context) const
{
	return Super::MakeOutgoingSpec(GameplayEffectClass, Level, Context);
}

FGameplayEffectContextHandle UBaseAbilitySystemComponent::MakeEffectContext() const
{
	return Super::MakeEffectContext();
}

FGameplayAbilitySpecHandle UBaseAbilitySystemComponent::GiveAbility(const FGameplayAbilitySpec& Spec)
{
	return Super::GiveAbility(Spec);
}

TArray<TPair<float, float>> UBaseAbilitySystemComponent::GetActiveEffectsTimeRemainingAndDuration(const FGameplayEffectQuery& Query) const
{
	return Super::GetActiveEffectsTimeRemainingAndDuration(Query);
}

bool UBaseAbilitySystemComponent::GetCharacterAbilitiesGiven()
{
	return CharacterAbilitiesGiven;
}

void UBaseAbilitySystemComponent::ClearAbility(const FGameplayAbilitySpecHandle& Handle)
{
	Super::ClearAbility(Handle);
}

void UBaseAbilitySystemComponent::CancelAllAbilities()
{
	Super::CancelAllAbilities();
}

void UBaseAbilitySystemComponent::AddLooseGameplayTag(const FGameplayTag& GameplayTag)
{
	Super::AddLooseGameplayTag(GameplayTag);
}
