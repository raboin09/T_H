// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Abilities/BaseGameplayAbility.h"
#include "BaseSummonedActor.generated.h"

UCLASS(Abstract)
class TACTICSHEROICS_API ABaseSummonedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseSummonedActor();

	virtual void Activate();
	bool ShouldActivateOnStart() { return bActivateOnStart; };
	class UStaticMeshComponent* GetMesh();
	virtual class UShapeComponent* GetCollisionComponent();

	UFUNCTION(BlueprintCallable)
		virtual void SetActorOwner(AActor* InActor);

	AActor* GetCharOwner() { return CharOwner; };

	UFUNCTION(BlueprintCallable)
		virtual void SetEffectContainerSpec(FBaseGameplayEffectContainerSpec InGameplayEffectContainerSpec);

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void HandleActorSpawnFX();

	UFUNCTION(BlueprintImplementableEvent)
		void HandleActorDeathFX();

	UFUNCTION(BlueprintImplementableEvent)
		void HandleActivationFX();

	UFUNCTION(BlueprintImplementableEvent)
		void HandleDeactivationFX();

private:



protected:

	UPROPERTY(EditDefaultsOnly)
		bool bActivateOnStart;

	UPROPERTY(EditDefaultsOnly)
		bool bHasVelocity;

	UPROPERTY(EditDefaultsOnly)
		UStaticMeshComponent* SummonedMesh;

	FBaseGameplayEffectContainerSpec GameplayEffectContainerSpec;

	AActor* CharOwner;

private:


};
