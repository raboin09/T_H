#include "Components/RTSSelectableComponent.h"

#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"

void URTSSelectableComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();

	if (!IsValid(Owner))
	{
		return;
	}

	UCapsuleComponent* CapsuleComponent = Owner->FindComponentByClass<UCapsuleComponent>();

	if (IsValid(CapsuleComponent))
	{
		DecalComponent = NewObject<UDecalComponent>(Owner, TEXT("SelectionCircleDecal"));

		if (DecalComponent)
		{
			DecalComponent->RegisterComponent();
			DecalComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

			FCollisionShape CollisionShape = CapsuleComponent->GetCollisionShape();
			float DecalHeight = CollisionShape.Capsule.HalfHeight * 2;
			float DecalRadius = CollisionShape.Capsule.Radius;

			DecalComponent->DecalSize = FVector(DecalHeight, DecalRadius, DecalRadius);

			DecalComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f)));

			SelectionCircleMaterialInstance = UMaterialInstanceDynamic::Create(SelectionCircleMaterial, this);
			DecalComponent->SetDecalMaterial(SelectionCircleMaterialInstance);

			DecalComponent->SetHiddenInGame(true);
		}
	}
}

void URTSSelectableComponent::SelectActor()
{
	if (bSelected)
	{
		return;
	}

	bSelected = true;

	// Update selection circle.
	if (IsValid(DecalComponent))
	{
		DecalComponent->SetHiddenInGame(false);
	}

	// Notify listeners.
	OnSelected.Broadcast();
}

void URTSSelectableComponent::DeselectActor()
{
	if (!bSelected)
	{
		return;
	}

	bSelected = false;

	// Update selection circles.
	if (IsValid(DecalComponent))
	{
		DecalComponent->SetHiddenInGame(true);
	}

	// Notify listeners.
	OnDeselected.Broadcast();
}

bool URTSSelectableComponent::IsSelected()
{
	return bSelected;
}
