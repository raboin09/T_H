// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "API/Actors/BaseObjectiveInterface.h"
#include "BaseObjectiveActor.generated.h"

class UBaseAbilitySystemComponent;

UCLASS()
class TACTICSHEROICS_API ABaseObjectiveActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABaseObjectiveActor();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override { return Cast<UAbilitySystemComponent>(AbilitySystemComponent); }

	UFUNCTION(BlueprintCallable)
		virtual void SetOwnerTeam(UBaseTeam* InTeam) { OwnerTeam = InTeam; };

	UFUNCTION(BlueprintCallable)
		UBaseTeam* GetOwnerTeam() const { return OwnerTeam; };

	

protected:
	virtual void BeginPlay() override;

public:	

protected:

	UBaseAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* ObjectiveMesh;

private:

	UBaseTeam* OwnerTeam;

};
