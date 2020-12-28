#include "RTSUtils.h"
#include "Landscape.h"
#include "EngineUtils.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Components/ShapeComponent.h"
#include "Components/ActorComponent.h"
#include "Engine/SCS_Node.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

float URTSUtils::GetActorDistance(AActor* First, AActor* Second, bool bConsiderCollisionSize)
{
	if (!First || !Second)
	{
		return 0.0f;
	}

	float Distance = First->GetDistanceTo(Second);

	if (bConsiderCollisionSize)
	{
		Distance -= GetActorCollisionSize(First) / 2.0f;
		Distance -= GetActorCollisionSize(Second) / 2.0f;
	}

	return Distance;
}

float URTSUtils::GetCollisionSize(TSubclassOf<AActor> ActorClass)
{
	AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
	return GetActorCollisionSize(DefaultActor) * DefaultActor->GetActorRelativeScale3D().X;
}

float URTSUtils::GetCollisionHeight(TSubclassOf<AActor> ActorClass)
{
	AActor* DefaultActor = ActorClass->GetDefaultObject<AActor>();
	return GetActorCollisionHeight(DefaultActor) * DefaultActor->GetActorRelativeScale3D().Z;
}

float URTSUtils::GetActorCollisionSize(AActor* Actor)
{
	if (!Actor)
	{
		return 0.0f;
	}

	UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();
	return GetShapeCollisionSize(ShapeComponent);
}

float URTSUtils::GetActorCollisionHeight(AActor* Actor)
{
	if (!Actor)
	{
		return 0.0f;
	}

	UShapeComponent* ShapeComponent = Actor->FindComponentByClass<UShapeComponent>();
	return GetShapeCollisionHeight(ShapeComponent);
}

float URTSUtils::GetShapeCollisionSize(UShapeComponent* ShapeComponent)
{
	if (!ShapeComponent)
	{
		return 0.0f;
	}

	FCollisionShape CollisionShape = ShapeComponent->GetCollisionShape();

	return CollisionShape.IsCapsule() ?
		CollisionShape.Capsule.Radius * 2 :
		FMath::Max(CollisionShape.Box.HalfExtentX, CollisionShape.Box.HalfExtentY) * 2;
}

float URTSUtils::GetShapeCollisionHeight(UShapeComponent* ShapeComponent)
{
	if (!ShapeComponent)
	{
		return 0.0f;
	}

	FCollisionShape CollisionShape = ShapeComponent->GetCollisionShape();

	return CollisionShape.IsCapsule() ?
		CollisionShape.Capsule.HalfHeight * 2 :
		CollisionShape.Box.HalfExtentZ * 2;
}

FVector URTSUtils::GetGroundLocation(UObject* WorldContextObject, FVector Location)
{
	if (!WorldContextObject)
	{
		return Location;
	}

	// Cast ray to hit world.
	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);
	TArray<FHitResult> HitResults;

	WorldContextObject->GetWorld()->LineTraceMultiByObjectType(
		HitResults,
		FVector(Location.X, Location.Y, 10000.0f),
		FVector(Location.X, Location.Y, -10000.0f),
		Params);

	for (auto& HitResult : HitResults)
	{
		if (HitResult.Actor != nullptr)
		{
			ALandscape* Landscape = Cast<ALandscape>(HitResult.Actor.Get());

			if (Landscape != nullptr)
			{
				return HitResult.Location;
			}

			continue;
		}

		return HitResult.Location;
	}

	return Location;
}

bool URTSUtils::IsSuitableLocationForActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location)
{
	if (!World)
	{
		return false;
	}

	UShapeComponent* ShapeComponent = FindDefaultComponentByClass<UShapeComponent>(ActorClass);

	if (!ShapeComponent)
	{
		return true;
	}

	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::AllDynamicObjects);

	return !World->OverlapAnyTestByObjectType(
		Location,
		FQuat::Identity,
		Params,
		ShapeComponent->GetCollisionShape());
}

UActorComponent* URTSUtils::FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass)
{
	AActor* ActorCDO = InActorClass->GetDefaultObject<AActor>();;
	UActorComponent* FoundComponent = ActorCDO->FindComponentByClass(InComponentClass);

	if (FoundComponent)
	{
		return FoundComponent;
	}

	// Check blueprint nodes. Components added in blueprint editor only (and not in code) are not available from CDO.
	UBlueprintGeneratedClass* ActorBlueprintGeneratedClass = Cast<UBlueprintGeneratedClass>(InActorClass);

	if (!ActorBlueprintGeneratedClass)
	{
		return nullptr;
	}

	const TArray<USCS_Node*>& ActorBlueprintNodes = ActorBlueprintGeneratedClass->SimpleConstructionScript->GetAllNodes();

	for (USCS_Node* Node : ActorBlueprintNodes)
	{
		if (Node->ComponentClass->IsChildOf(InComponentClass))
		{
			return Node->ComponentTemplate;
		}
	}

	return nullptr;
}
