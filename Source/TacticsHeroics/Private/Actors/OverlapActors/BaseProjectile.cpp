#include "Actors/OverlapActors/BaseProjectile.h"
#include "Utils/InGameUtils.h"
#include "TimerManager.h"
#include "Actors/BaseCharacter.h"
#include "Components/SphereComponent.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	SummonedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SummonedMesh->SetupAttachment(RootComponent);
	ExplosionRadius = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosiveRadius"));
	ExplosionRadius->SetupAttachment(RootComponent);
}

UShapeComponent* ABaseProjectile::GetCollisionComponent()
{
	return CollisionComponent;
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	FGameplayTag TempTag;
	ActivateCombatActor(TempTag);
}

void ABaseProjectile::HandleOverlapEvent(FGameplayEventData EventData)
{
	Super::HandleOverlapEvent(EventData);
	if (EventData.Instigator != EventData.Target) {
		TArray<FHitResult> HitResults;
		if (bExplodesOnDeath) {
			ExplosionRadius->GetOverlappingActors(HitActors, ABaseCharacter::StaticClass());
		}
		UInGameUtils::ApplyExternalEffectContainerSpec(UInGameUtils::AddTargetsToEffectContainerSpec(GameplayEffectContainerSpec, HitResults, HitActors));
		if (bDiesUponHit) {
			if (DeathBuffer > 0) {
				FTimerHandle DeathHandle;
				GetWorldTimerManager().SetTimer(DeathHandle, this, &ABaseProjectile::DestroyProjectile, DeathBuffer, false);
			}
			else {
				DestroyProjectile();
			}
		}
	}
}

void ABaseProjectile::DestroyProjectile()
{
	HandleActorDeathFX();
	GetWorld()->DestroyActor(this);
}
