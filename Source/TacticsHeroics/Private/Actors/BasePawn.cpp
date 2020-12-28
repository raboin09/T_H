#include "BasePawn.h"

ABasePawn::ABasePawn()
{
	PrimaryActorTick.bCanEverTick = false;
}
/*
void ABasePawn::BeginPlay()
{
	Super::BeginPlay();	
}

void ABasePawn::AddPawnAbilities()
{
	return;
}

void ABasePawn::AddStartupEffects()
{

}

void ABasePawn::InitializeAttributes()
{

}

void ABasePawn::BindAbilityInput()
{

}

void ABasePawn::MoveForward(float Value)
{

}

void ABasePawn::MoveRight(float Value)
{

}

void ABasePawn::Turn(float Value)
{

}

void ABasePawn::TurnAtRate(float Rate)
{

}

void ABasePawn::Lookup(float Value)
{

}

void ABasePawn::LookUpAtRate(float Rate)
{

}

void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ABasePawn::GetHealth() const
{
	if (AttributeBaseSet) {
		return AttributeBaseSet->GetHealth();
	}
	return 0.0f;
}

float ABasePawn::GetMaxHealth() const
{
	if (AttributeBaseSet) {
		return AttributeBaseSet->GetMaxHealth();
	}
	return 0.0f;
}

float ABasePawn::GetCurrentMoveSpeed() const
{
	if (AttributeBaseSet) {
		return AttributeBaseSet->GetMoveSpeed();
	}
	return 0.0f;
}

float ABasePawn::GetBaseMoveSpeed() const
{
	if (AttributeBaseSet) {
		return AttributeBaseSet->GetMoveSpeed();
	}
	return 0.0f;
}

float ABasePawn::GetStamina() const
{
	if (AttributeBaseSet) {
		return AttributeBaseSet->GetStamina();
	}
	return 0.0f;
}

float ABasePawn::GetMaxStamina() const
{
	if (AttributeBaseSet) {
		return AttributeBaseSet->GetMaxStamina();
	}
	return 0.0f;
}

bool ABasePawn::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation = true)
{
	return false;
}

void ABasePawn::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UBaseGameplayAbility*>& ActiveAbilities)
{
	return;
}

bool ABasePawn::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	return false;
}

void ABasePawn::RemoveAbilities()
{
	return;
}

void ABasePawn::Die()
{
	return;
}

bool ABasePawn::IsAlive()
{
	return false;
}

void ABasePawn::SetHealth(float InHealth)
{
	return;
}

void ABasePawn::SetStamina(float InStamina)
{
	return;
}

void ABasePawn::SetOwnerTeam(TScriptInterface<IBaseTeamInterface> InTeam)
{
	OwnerTeam = InTeam;
}

FName ABasePawn::GetPawnName() const
{
	return PawnName;
}

FCharacterCombatAbilitiesWidgetDataStruct ABasePawn::GetPawnAbilityInfo() const
{
	return AbilityIconInfo;
}

void ABasePawn::FinishDying()
{
	return;
}
*/

