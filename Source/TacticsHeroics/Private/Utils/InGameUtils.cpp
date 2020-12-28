#include "Utils/InGameUtils.h"
#include "Abilities/BaseGameplayAbility.h"
#include "GameplayAbilityTypes.h"

FBaseGameplayEffectContainerSpec UInGameUtils::AddTargetsToEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec, const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
	FBaseGameplayEffectContainerSpec NewSpec = ContainerSpec;
	NewSpec.AddTargets(HitResults, TargetActors);
	return NewSpec;
}

TArray<FActiveGameplayEffectHandle> UInGameUtils::ApplyExternalEffectContainerSpec(const FBaseGameplayEffectContainerSpec& ContainerSpec)
{
	TArray<FActiveGameplayEffectHandle> AllEffects;
	for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecs)
	{
		if (SpecHandle.IsValid())
		{
			for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
			{
				AllEffects.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
			}
		}
	}
	return AllEffects;
}

USoundCue* UInGameUtils::PickRandomSoundInArray(TArray<USoundCue*> InArray)
{
	if (InArray.Num() > 0) {
		int8 Index = FMath::RandRange(0, InArray.Num() - 1);
		return InArray[Index];
	}
	return nullptr;
}
