// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "RTSUtils.generated.h"

class AActor;
class UWorld;
class UShapeComponent;

/**
 * 
 */
UCLASS()
class TACTICSHEROICS_API URTSUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Gets the distance between the two specified actors, optionally subtracting their collision sizes. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetActorDistance(AActor* First, AActor* Second, bool bConsiderCollisionSize);

	/** Gets the approximated collision size of actors of the specified class. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetCollisionSize(TSubclassOf<AActor> ActorClass);

	/** Gets the approximated collision height of actors of the specified class. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetCollisionHeight(TSubclassOf<AActor> ActorClass);

	/** Gets the approximated collision size of the specified actor. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetActorCollisionSize(AActor* Actor);

	/** Gets the approximated collision height of the specified actor. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetActorCollisionHeight(AActor* Actor);

	/** Gets the approximated collision size of the specified shape. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetShapeCollisionSize(UShapeComponent* ShapeComponent);

	/** Gets the approximated collision height of the specified shape. */
	UFUNCTION(BlueprintPure, Category = "RTS")
		static float GetShapeCollisionHeight(UShapeComponent* ShapeComponent);

	UFUNCTION(BlueprintPure, Category = "RTS", meta = (WorldContext = "WorldContextObject"))
		static FVector GetGroundLocation(UObject* WorldContextObject, FVector Location);

	/**
	* Checks whether the specified actor can be placed at the passed location.
	*/
	UFUNCTION(BlueprintCallable, Category = "RTS")
		static bool IsSuitableLocationForActor(UWorld* World, TSubclassOf<AActor> ActorClass, const FVector& Location);

	template<class T>
	static T* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass)
	{
		return (T*)FindDefaultComponentByClass(InActorClass, T::StaticClass());
	}

	UFUNCTION(BlueprintCallable, Category = "RTS")
		static UActorComponent* FindDefaultComponentByClass(const TSubclassOf<AActor> InActorClass, const TSubclassOf<UActorComponent> InComponentClass);

	
};
