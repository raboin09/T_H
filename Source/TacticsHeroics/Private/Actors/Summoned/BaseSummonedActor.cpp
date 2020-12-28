#include "Actors/Summoned/BaseSummonedActor.h"
#include "Actors/BaseCharacter.h"
#include "Actors/Weapons/BaseWeaponActor.h"
#include "API/Abilities/BaseGameplayAbilityInterface.h"
#include "Components/SphereComponent.h"
#include "Components/ShapeComponent.h"

ABaseSummonedActor::ABaseSummonedActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseSummonedActor::Activate()
{
	HandleActivationFX();
}

void ABaseSummonedActor::SetEffectContainerSpec(FBaseGameplayEffectContainerSpec InGameplayEffectContainerSpec)
{
	GameplayEffectContainerSpec = InGameplayEffectContainerSpec;
}

UStaticMeshComponent* ABaseSummonedActor::GetMesh()
{
	return SummonedMesh;
}

UShapeComponent* ABaseSummonedActor::GetCollisionComponent()
{
	return nullptr;
}

void ABaseSummonedActor::SetActorOwner(AActor* InActor)
{
	CharOwner = InActor;
	GetCollisionComponent()->IgnoreActorWhenMoving(CharOwner, true);
	GetCollisionComponent()->IgnoreComponentWhenMoving(GetMesh(), true);
	GetCollisionComponent()->IgnoreActorWhenMoving(GetInstigator(), true);
	ABaseCharacter* TempCharOwner = Cast<ABaseCharacter>(CharOwner);
	if (TempCharOwner) {
		GetCollisionComponent()->IgnoreComponentWhenMoving(TempCharOwner->GetMesh(), true);
		GetCollisionComponent()->IgnoreActorWhenMoving(TempCharOwner->GetCurrentWeapon(), true);
	}
	if (bActivateOnStart) {
		Activate();
	}
}

void ABaseSummonedActor::BeginPlay()
{
	Super::BeginPlay();
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCollisionComponent()->SetGenerateOverlapEvents(false);
	HandleActorSpawnFX();
}

