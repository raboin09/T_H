#include "Abilities/TargetTypes/BaseTargetType.h"
#include "API/Actors/BasePawnInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "BaseCharacter.h"

void UBaseTargetType::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

bool UBaseTargetType::IsValidTarget(APawn* TargetingCharacter, const AActor* TargetingActor, EValidTargetType InType) const
{
	// OutActors.Add(const_cast<AActor*>(EventData.Target));
	ABaseCharacter* SourcePawnInterface = Cast<ABaseCharacter>(TargetingCharacter);
	ABaseCharacter* DestinationActorInterface = Cast<ABaseCharacter>(const_cast<AActor*>(TargetingActor));
	switch (InType)
	{
	case EValidTargetType::ALLIES:
		if (SourcePawnInterface && DestinationActorInterface) {
			return SourcePawnInterface->GetOwnerTeam() == DestinationActorInterface->GetOwnerTeam();
		}
		return false;
		break;
	case EValidTargetType::ENEMIES:
		if (SourcePawnInterface && DestinationActorInterface) {
			return SourcePawnInterface->GetOwnerTeam() != DestinationActorInterface->GetOwnerTeam();
		}
		return false;
		break;
	case EValidTargetType::ALL:
		return true;
		break;
	}
	return false;
}

void UBaseTargetType_UseOwner::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void UBaseTargetType_UseEventData_ALL::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}

void UBaseTargetType_ArcHitbox::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	float DegreeIncrements = ConeTraceArcWidth / NumCones;
	float YawValueFirst = ConeTraceArcWidth * -.5;
	TArray<AActor*> AlreadyHitActors;
	for (int i = 0; i < NumCones; i++) {
		float YawValueSecond = DegreeIncrements * i;
		float YawFinal = YawValueFirst + YawValueSecond;
		FVector RotatedVector = TargetingCharacter->GetActorForwardVector().RotateAngleAxis(YawFinal, FVector(0, 0, 1)) * ArcDistance;
		FVector CharOwnerLocation = TargetingCharacter->GetActorLocation();
		CharOwnerLocation.Z += 50;
		FVector FinalVectorInput = RotatedVector + CharOwnerLocation;
		TArray<TEnumAsByte <EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(TargetingCharacter);
		TArray<FHitResult> HitResults;
		if (bShowDebug) {
			UKismetSystemLibrary::SphereTraceMultiForObjects(TargetingCharacter, CharOwnerLocation, FinalVectorInput, SweepRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::ForDuration, HitResults, true);
		}
		else {
			UKismetSystemLibrary::SphereTraceMultiForObjects(TargetingCharacter, CharOwnerLocation, FinalVectorInput, SweepRadius, ObjectTypes, false, IgnoreActors, EDrawDebugTrace::None, HitResults, true);
		}
		
		for (FHitResult HitResult : HitResults) {
			AActor* HitActor = HitResult.Actor.Get();
			if (HitActor && !AlreadyHitActors.Contains(HitActor) && IsValidTarget(TargetingCharacter, HitActor, ValidTarget)) {
				OutHitResults.Add(HitResult);
				AlreadyHitActors.Add(HitActor);
			}
		}
	}

}

void UBaseTargetType_UseEventData_ALLIES::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (IsValidTarget(TargetingCharacter, EventData.Target, EValidTargetType::ALLIES)) {
		if (FoundHitResult)
		{
			OutHitResults.Add(*FoundHitResult);
		}
		else if (EventData.Target)
		{
			OutActors.Add(const_cast<AActor*>(EventData.Target));
		}
	}
}

void UBaseTargetType_UseEventData_ENEMIES::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (IsValidTarget(TargetingCharacter, EventData.Target, EValidTargetType::ENEMIES)) {
		if (FoundHitResult)
		{
			OutHitResults.Add(*FoundHitResult);
		}
		else if (EventData.Target)
		{
			OutActors.Add(const_cast<AActor*>(EventData.Target));
		}
	}
}

void UBaseTargetType_UseEventData_SELF::GetTargets_Implementation(APawn* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (TargetingCharacter == EventData.Target) {
		if (FoundHitResult)
		{
			OutHitResults.Add(*FoundHitResult);
		}
		else if (EventData.Target)
		{
			OutActors.Add(const_cast<AActor*>(EventData.Target));
		}
	}
}
