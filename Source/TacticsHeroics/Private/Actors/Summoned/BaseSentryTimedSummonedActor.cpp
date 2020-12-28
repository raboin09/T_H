#include "Actors/Summoned/BaseSentryTimedSummonedActor.h"
#include "Actors/OverlapActors/BaseProjectile.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/KismetMathLibrary.h"

ABaseSentryTimedSummonedActor::ABaseSentryTimedSummonedActor()
{
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	SummonedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SummonedMesh->SetupAttachment(RootComponent);
}

void ABaseSentryTimedSummonedActor::Activate()
{
	Super::Activate();
	InitiateFiringLoop();
}

UShapeComponent* ABaseSentryTimedSummonedActor::GetCollisionComponent()
{
	return CollisionComponent;
}

void ABaseSentryTimedSummonedActor::InitiateFiringLoop()
{
	FTimerHandle DelayHandle;
	GetWorldTimerManager().SetTimer(DelayHandle, this, &ABaseSentryTimedSummonedActor::Fire, 0.1f, false);
}

void ABaseSentryTimedSummonedActor::Fire()
{
	if (bIsAlive) {
		FTimerHandle DelayHandle;
		GetWorldTimerManager().SetTimer(DelayHandle, this, &ABaseSentryTimedSummonedActor::Fire, ShootCooldown, false);
		FVector StartLocation = GetActorLocation() + (UKismetMathLibrary::RandomUnitVector() * FMath::Clamp(SpawnBase, SpawnZMin, SpawnZMax));
		StartLocation.Z += 100;
		FRotator StartRotation = FRotator(UKismetMathLibrary::RandomFloatInRange(PitchMin, PitchMax), UKismetMathLibrary::RandomFloatInRange(YawMin, YawMax), UKismetMathLibrary::RandomFloatInRange(RollMin, RollMax));
		FVector EndLocation = UKismetMathLibrary::GetForwardVector(StartRotation) * 50000;
		FHitResult OutHit;
		ABaseProjectile* SpawnedProjecile = nullptr;
		if (GetWorld()->LineTraceSingleByChannel(OutHit, StartLocation, EndLocation, ECollisionChannel::ECC_WorldStatic)) {
			if ((StartLocation - OutHit.ImpactPoint).Size() > 200.f) {
				FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, OutHit.Location);
				SpawnedProjecile = SpawnProjectile(StartLocation, SpawnRotation);
			}
		}
		else {
			SpawnedProjecile = SpawnProjectile(StartLocation, StartRotation);
		}
		if (SpawnedProjecile) {
			HandleSpawnProjectileFX(StartLocation);
			SpawnedProjecile->GetCollisionComponent()->IgnoreActorWhenMoving(GetOwner(), true);
			SpawnedProjecile->SetActorOwner(this);
			SpawnedProjecile->SetEffectContainerSpec(GameplayEffectContainerSpec);
			if (SpawnedProjecile->ShouldActivateOnStart()) {
				SpawnedProjecile->Activate(); 
			}
		}
	}
}

ABaseProjectile* ABaseSentryTimedSummonedActor::SpawnProjectile(FVector SpawnLocation, FRotator SpawnRotation)
{
	return GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
}
