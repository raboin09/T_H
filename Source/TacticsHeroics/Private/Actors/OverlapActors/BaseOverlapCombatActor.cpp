#include "Actors/OverlapActors/BaseOverlapCombatActor.h"
#include "Actors/BaseCharacter.h"
#include "Sound/SoundCue.h"
#include "TimerManager.h"
#include "Components/ShapeComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseOverlapCombatActor::ABaseOverlapCombatActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

UShapeComponent* ABaseOverlapCombatActor::GetCollisionComponent()
{
	return nullptr;
}

UMeshComponent* ABaseOverlapCombatActor::GetMesh()
{
	return nullptr;
}

void ABaseOverlapCombatActor::BeginPlay()
{
	Super::BeginPlay();	
	GetCollisionComponent()->SetGenerateOverlapEvents(false);
	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseOverlapCombatActor::ActorBeginOverlap);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ABaseOverlapCombatActor::ActorEndOverlap);
	GetCollisionComponent()->OnComponentHit.AddDynamic(this, &ABaseOverlapCombatActor::OnHit);
	HandleReset();
}

void ABaseOverlapCombatActor::HandleOverlapEvent(FGameplayEventData EventData)
{
	
}

void ABaseOverlapCombatActor::HandleReset()
{
	bDone = false;
	HitActors.Empty();
	DeactivateCombatActor();
}

void ABaseOverlapCombatActor::ActivateCombatActor(FGameplayTag InTag)
{
	if (!bIsActive) {
		bIsActive = true;
		AttackEventTag = InTag;
		GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		GetCollisionComponent()->SetGenerateOverlapEvents(true);
	}
}

void ABaseOverlapCombatActor::DeactivateCombatActor()
{
	bIsActive = false;
	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCollisionComponent()->SetGenerateOverlapEvents(false);
}

void ABaseOverlapCombatActor::Activate()
{
	Super::Activate();
	// TODO Alter logic to fit BaseSummonedActor framework
}

void ABaseOverlapCombatActor::ActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CharOwner != NULL && bIsActive && OtherActor && OtherComp != SummonedMesh && OtherActor != GetCharOwner() && bIsActive && !bDone && !OtherActor->IsA(ABaseSummonedActor::StaticClass()) && !HitActors.Contains(OtherActor)) {
		bDone = true;
		HitActors.Add(OtherActor);
		FGameplayEventData EventData;
		EventData.Target = OtherActor;
		EventData.Instigator = GetInstigator();
		HandleOverlapFX(SweepResult.ImpactPoint);
		HandleOverlapEvent(EventData);
		HandleReset();
	}
}

void ABaseOverlapCombatActor::ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ABaseOverlapCombatActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (CharOwner != NULL && bIsActive && OtherActor != this && OtherActor != GetCharOwner() && bDiesUponHit && !OtherActor->IsA(ABaseSummonedActor::StaticClass()))
	{
		HitActors.Add(OtherActor);
		FGameplayEventData EventData;
		EventData.Target = OtherActor;
		EventData.Instigator = GetInstigator();
		HandleOverlapFX(GetActorLocation());
		HandleOverlapEvent(EventData);
	}
}
