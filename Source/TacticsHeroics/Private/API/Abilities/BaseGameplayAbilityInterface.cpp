#include "BaseGameplayAbilityInterface.h"
#include "AbilitySystemComponent.h"
#include "BaseCharacter.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "Abilities/TargetTypes/BaseTargetType.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "BaseAbilitySystemComponent.h"

bool FBaseGameplayEffectContainerSpec::HasValidEffects() const
{
    return TargetGameplayEffectSpecs.Num() > 0;
}

bool FBaseGameplayEffectContainerSpec::HasValidTargets() const
{
    return TargetData.Num() > 0;
}

void FBaseGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults, const TArray<AActor*>& TargetActors)
{
    for (const FHitResult& HitResult : HitResults)
    {
        FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
        TargetData.Add(NewData);
    }

    if (TargetActors.Num() > 0)
    {
        FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
        NewData->TargetActorArray.Append(TargetActors);
        TargetData.Add(NewData);
    }
}
