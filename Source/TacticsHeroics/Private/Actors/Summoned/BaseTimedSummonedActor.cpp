#include "Actors/Summoned/BaseTimedSummonedActor.h"
#include "TimerManager.h"

void ABaseTimedSummonedActor::Activate()
{
	Super::Activate();
	bIsAlive = true;
	FTimerHandle DelayHandle;
	GetWorldTimerManager().SetTimer(DelayHandle, this, &ABaseTimedSummonedActor::OnDeactivation, TimeAlive, false);
}

void ABaseTimedSummonedActor::OnDeactivation()
{
	Super::HandleDeactivationFX();
	GetWorld()->DestroyActor(this);
	bIsAlive = false;
}
