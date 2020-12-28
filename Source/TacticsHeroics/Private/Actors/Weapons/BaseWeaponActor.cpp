#include "BaseWeaponActor.h"
#include "Utils/InGameUtils.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"

ABaseWeaponActor::ABaseWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	SummonedMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SummonedMesh->SetupAttachment(RootComponent);
}

void ABaseWeaponActor::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseWeaponActor::HandleOverlapEvent(FGameplayEventData EventData)
{
	Super::HandleOverlapEvent(EventData);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetCharOwner(), AttackEventTag, EventData);
	FGameplayEventData TEmpoEventData;
}

