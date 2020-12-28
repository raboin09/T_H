// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/BaseCharacter.h"
#include "Actors/Summoned/BaseSummonedActor.h"
#include "GameplayAbilityTypes.h"
#include "BaseOverlapCombatActor.generated.h"

class UShapeComponent;
class USoundCue;
class APointLight;

UCLASS(Abstract)
class TACTICSHEROICS_API ABaseOverlapCombatActor : public ABaseSummonedActor
{
	GENERATED_BODY()
	
public:	
	ABaseOverlapCombatActor();

	UFUNCTION(BlueprintCallable)
		virtual void ActivateCombatActor(FGameplayTag InTag);

	UFUNCTION(BlueprintCallable)
		virtual void DeactivateCombatActor();

	void Activate() override;

	virtual UShapeComponent* GetCollisionComponent();
	virtual UMeshComponent* GetMesh();

	UFUNCTION()
		void ActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void ActorEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

protected:
	virtual void BeginPlay() override;
	virtual void HandleOverlapEvent(FGameplayEventData EventData);

	UFUNCTION(BlueprintImplementableEvent)
		void HandleOverlapFX(FVector InLocation);

private:

	virtual void HandleReset();
	void PlayImpactFX(FVector_NetQuantize ImpactPoint);

protected:

	UPROPERTY(EditDefaultsOnly)
		bool bDiesUponHit;

	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* ImpactVisualFX;

	UPROPERTY(EditDefaultsOnly)
		USoundCue* ImpactSound;

	UPROPERTY(EditDefaultsOnly)
		FGameplayTag AttackEventTag;

	TArray<AActor*> HitActors;

private:
	bool bIsActive;
	bool bDone;
};
