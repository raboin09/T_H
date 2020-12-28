#include "Abilities/AbilityArchetypes/WeaponGameplayAbility.h"
#include "Abilities/BaseGameplayAbility.h"
#include "Actors/BaseCharacter.h"
#include "Actors/Weapons/BaseWeaponActor.h"
#include "Abilities/Tasks/Task_PlayMontageAndWait.h"

void UWeaponGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (CommitAbility(Handle, ActorInfo, ActivationInfo)) {
			UTask_PlayMontageAndWait* CurrTask = UTask_PlayMontageAndWait::PlayMontageAndWaitForEvent(this, FName("None"), MontageToPlay, MontageEventTags);
			CurrTask->OnBlendOut.AddDynamic(this, &UWeaponGameplayAbility::OnCompleted);
			CurrTask->OnInterrupted.AddDynamic(this, &UWeaponGameplayAbility::OnCancelled);
			CurrTask->OnCancelled.AddDynamic(this, &UWeaponGameplayAbility::OnCancelled);
			CurrTask->OnCompleted.AddDynamic(this, &UWeaponGameplayAbility::OnCompleted);
			CurrTask->EventReceived.AddDynamic(this, &UWeaponGameplayAbility::EventReceived);
			CurrTask->Activate();
		}
		else {
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
}

void UWeaponGameplayAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EventReceived(EventTag, EventData);
	ApplyEffectContainer(EventTag, EventData);
}
