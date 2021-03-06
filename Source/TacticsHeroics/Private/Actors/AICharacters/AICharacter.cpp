// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/AICharacters/AICharacter.h"
#include "Player/BasePlayerState.h"

AAICharacter::AAICharacter()
{

}

void AAICharacter::SetupAbilitySystem()
{
	ABasePlayerState* BasePlayerState = GetPlayerState<ABasePlayerState>();
	if (BasePlayerState)
	{
		AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(BasePlayerState->GetAbilitySystemComponent());
		BasePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(BasePlayerState, this);

		InitializeAttributes();
		AddStartupEffects();
		AddCharacterAbilities();

		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);
	}
}
