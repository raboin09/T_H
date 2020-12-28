#include "BaseGameplayAbility.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "BaseCharacter.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "Abilities/TargetTypes/BaseTargetType.h"
#include "BaseAbilitySystemComponent.h"
#include "GameplayCueManager.h"
#include "GameplayTagContainer.h"
  
UBaseGameplayAbility::UBaseGameplayAbility(){ }

void UBaseGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (ActivateAbilityOnGranted)
	{
		bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UBaseGameplayAbility::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FCollisionQueryParams Test;
}

void UBaseGameplayAbility::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	FCollisionQueryParams Test;
}

void UBaseGameplayAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	
}

FBaseGameplayEffectContainerSpec UBaseGameplayAbility::MakeEffectContainerSpecFromContainer(const FBaseGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FBaseGameplayEffectContainerSpec ReturnSpec;
	AActor* OwningActor = GetAvatarActorFromActorInfo();
	ABaseCharacter* OwningCharacter = Cast<ABaseCharacter>(OwningActor);
	UBaseAbilitySystemComponent* OwningASC = UBaseAbilitySystemComponent::GetAbilitySystemComponentFromActor(OwningActor);

	if (OwningASC)
	{
		if (Container.TargetType.Get())
		{
			TArray<FHitResult> HitResults;
			TArray<AActor*> TargetActors;
			const UBaseTargetType* TargetTypeCDO = Container.TargetType.GetDefaultObject();
			AActor* AvatarActor = GetAvatarActorFromActorInfo();
			TargetTypeCDO->GetTargets(OwningCharacter, AvatarActor, EventData, HitResults, TargetActors);
			ReturnSpec.AddTargets(HitResults, TargetActors);
		}

		if (OverrideGameplayLevel == INDEX_NONE)
		{
			OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel();
		}

		for (const TSubclassOf<UGameplayEffect>& EffectClass : Container.TargetGameplayEffectClasses)
		{
			ReturnSpec.TargetGameplayEffectSpecs.Add(MakeOutgoingGameplayEffectSpec(EffectClass, OverrideGameplayLevel));
		}
	}
	return ReturnSpec;
}

FBaseGameplayEffectContainerSpec UBaseGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FBaseGameplayEffectContainer* FoundContainer = EffectContainerMap.Find(ContainerTag);
	if (FoundContainer)
	{
		return MakeEffectContainerSpecFromContainer(*FoundContainer, EventData, OverrideGameplayLevel);
	}	
	return FBaseGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> UBaseGameplayAbility::ApplyEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		AllEffects.Append(K2_ApplyGameplayEffectSpecToTarget(SpecHandle, ContainerSpec.TargetData));
	}
	return AllEffects;
}

TArray<FActiveGameplayEffectHandle> UBaseGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
	FBaseGameplayEffectContainerSpec Spec = MakeEffectContainerSpec(ContainerTag, EventData, OverrideGameplayLevel);
	return ApplyEffectContainerSpec(Spec);
}
