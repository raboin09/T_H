// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "Abilities/BaseAttributeSet.h"
#include "API/Actors/BasePawnInterface.h"
#include "UObject/Interface.h"
#include "BaseCharacterInterface.generated.h"

class UBaseAbilitySystemComponent;
class UBaseGameplayAbility;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UBaseCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

class TACTICSHEROICS_API IBaseCharacterInterface
{
	GENERATED_BODY()

public:

};
