#include "Abilities/AbilityArchetypes/SpawnSingleProjectileAbility.h"
#include "Actors/BaseCharacter.h"
#include "Actors/Weapons/BaseWeaponActor.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/OverlapActors/BaseProjectile.h"
#include "Abilities/Tasks/Task_PlayMontageAndWait.h"

void USpawnSingleProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (CommitAbility(Handle, ActorInfo, ActivationInfo)) {
			UTask_PlayMontageAndWait* CurrTask = UTask_PlayMontageAndWait::PlayMontageAndWaitForEvent(this, FName("None"), MontageToPlay, MontageEventTags);
			CurrTask->OnBlendOut.AddDynamic(this, &USpawnSingleProjectileAbility::OnCompleted);
			CurrTask->OnInterrupted.AddDynamic(this, &USpawnSingleProjectileAbility::OnCancelled);
			CurrTask->OnCancelled.AddDynamic(this, &USpawnSingleProjectileAbility::OnCancelled);
			CurrTask->OnCompleted.AddDynamic(this, &USpawnSingleProjectileAbility::OnCompleted);
			CurrTask->EventReceived.AddDynamic(this, &USpawnSingleProjectileAbility::EventReceived);
			CurrTask->Activate();
		}
		else {
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
		}
	}
}

void USpawnSingleProjectileAbility::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Super::EventReceived(EventTag, EventData);
	
	ABaseCharacter* AbilityOwner = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (AbilityOwner) {
		FVector StartLocation = AbilityOwner->GetMesh()->GetSocketTransform(SocketNameToSpawnAt).GetLocation();
		FVector EyeLocation;
		FRotator EyeRotation;
		AbilityOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);
		FVector ShotDirection = EyeRotation.Vector();

		float HalfRad = FMath::DegreesToRadians(2.0f);
		ShotDirection = FMath::VRandCone(ShotDirection, HalfRad, HalfRad);
		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(AbilityOwner);
		QueryParams.AddIgnoredActor(AbilityOwner->GetCurrentWeapon());
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;
		ABaseSummonedActor* SpawnedActor = nullptr;
		FHitResult OutHit;
		SpawnedActor = SpawnProjectile(StartLocation, EyeRotation);
		if (SpawnedActor) {
			SpawnedActor->SetActorOwner(AbilityOwner);
			SpawnedActor->SetEffectContainerSpec(MakeEffectContainerSpec(EventTag, EventData));
		}
	}

	/*
	ABaseCharacter* AbilityOwner = Cast<ABaseCharacter>(GetAvatarActorFromActorInfo());
	if (AbilityOwner) {
		FVector AL = AbilityOwner->GetActorLocation();
		FVector StartLocation = AbilityOwner->GetMesh()->GetSocketTransform(SocketNameToSpawnAt).GetLocation();
		FRotator CameraRotation = AbilityOwner->GetFollowCamera()->K2_GetComponentRotation();
		FVector EndLocation;
		ABaseSummonedActor* SpawnedActor = nullptr;
		if (bSpawnedActorHasVelocity) {
			EndLocation = UKismetMathLibrary::GetForwardVector(CameraRotation) * SpawnOffset;
			FHitResult OutHit;
			if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECC_Visibility)) {
				FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, OutHit.Location);
				SpawnRotation.Pitch += PitchAdjust;
				SpawnedActor = SpawnProjectile(StartLocation, SpawnRotation);
			}
			else {
				SpawnedActor = SpawnProjectile(StartLocation, CameraRotation);
			}
		}
		else {
			SpawnedActor = SpawnProjectile(StartLocation, FRotator::ZeroRotator);
		}

		if (SpawnedActor) {
			SpawnedActor->SetActorOwner(AbilityOwner);
			SpawnedActor->SetEffectContainerSpec(MakeEffectContainerSpec(EventTag, EventData));
		}
	}
	*/
}

ABaseSummonedActor* USpawnSingleProjectileAbility::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation)
{
	UWorld* CurrWorld = GetWorld();
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Owner = GetAvatarActorFromActorInfo();
	if (CurrWorld) {
		ABaseSummonedActor* SpawnedActor = GetWorld()->SpawnActor<ABaseSummonedActor>(ProjectileClass, SpawnLocation, SpawnRotation, Params);
		return SpawnedActor;
	}
	else {
		return nullptr;
	}
}
