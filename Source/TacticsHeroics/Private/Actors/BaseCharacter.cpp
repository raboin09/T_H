#include "BaseCharacter.h"
#include "Abilities/BaseAttributeSet.h"
#include "BaseAbilitySystemComponent.h"
#include "Player/BasePlayerState.h"
#include "BaseGameplayAbility.h"
#include "Camera/CameraComponent.h"
#include "BaseWeaponActor.h"
#include "AI/BaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "Data/Character/CharacterAddonsData.h"
#include "TimerManager.h"
#include "Abilities/AbilityArchetypes/WeaponGameplayAbility.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameplayTagContainer.h"
#include "Player/BasePlayerController.h"
#include "Components/InputComponent.h"
#include "GameplayTagContainer.h"
#include "Components/RTSSelectableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Controller.h"
#include "Sound/SoundCue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayAbilitySpec.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bAlwaysRelevant = true;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = true;

	SelectComponent = CreateDefaultSubobject<URTSSelectableComponent>(TEXT("SelectionComponent"));
}

void ABaseCharacter::BlockVoicesForDuration(USoundCue* InSoundCue)
{
	FTimerHandle TimerHandle;
	bCanPlayVoices = false;
	if (InSoundCue) {
		float DelayTime = InSoundCue->GetDuration();
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
			bCanPlayVoices = true;
		}, DelayTime, 0.f);
	}
	else {
		bCanPlayVoices = true;
	}
}

void ABaseCharacter::BeginPlay()
{
	bCanAttack = true;
	bCanPlayVoices = true;
	Super::BeginPlay();
	if (AddonDataAsset) {
		Addons = AddonDataAsset->AddonStruct;
		DefaultFaction = AddonDataAsset->DefaultFaction;
		PlayEntranceVoice();
	}
	// SwitchOffDecal();
	InitWeaponActor();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::Lookup);

	PlayerInputComponent->BindAction("Jump", IE_Pressed,  this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

}

void ABaseCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetupAbilitySystem();
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::AddCharacterAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->GetCharacterAbilitiesGiven())
	{
		return;
	}
	if (Addons.Weapon_1)	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Addons.Weapon_1));
	if (Addons.Trinket_1)	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Addons.Trinket_1));
	if (Addons.Ability_1)	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Addons.Ability_1));
	if (Addons.Ability_2)	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Addons.Ability_2));
	if (Addons.Ability_3)	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Addons.Ability_3));
	if (Addons.Ability_4)	AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(Addons.Ability_4));

	AbilitySystemComponent->SetCharacterAbilitiesGiven(true);
}

void ABaseCharacter::AddStartupEffects()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || AbilitySystemComponent->GetStartupEffectsApplied())
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf<UGameplayEffect> GameplayEffect : Addons.StartupEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
		}
	}

	AbilitySystemComponent->SetStartupEffectsApplied(true);
}

void ABaseCharacter::InitializeAttributes()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (!Addons.DefaultAttributes)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(Addons.DefaultAttributes, 1.0f, EffectContext);
	if (NewHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), AbilitySystemComponent);
	}
}

void ABaseCharacter::RemoveAbilities()
{
	if (GetLocalRole() != ROLE_Authority || !AbilitySystemComponent || !AbilitySystemComponent->GetCharacterAbilitiesGiven())
	{
		return;
	}  

	TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		// Add check to see if Addons contains Spec.Ability->GetClass()
		if ((Spec.SourceObject == this))
		{
			AbilitiesToRemove.Add(Spec.Handle);
		}
	}

	for (int32 i = 0; i < AbilitiesToRemove.Num(); i++)
	{
		AbilitySystemComponent->ClearAbility(AbilitiesToRemove[i]);
	}
	AbilitySystemComponent->SetCharacterAbilitiesGiven(false);
}

void ABaseCharacter::Die()
{
	RemoveAbilities();

	PlayDeathVoice();

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->Velocity = FVector(0);

	OnCharacterDied.Broadcast(this);
	ABaseAIController* CurrController = Cast<ABaseAIController>(Controller);
	if (CurrController) {
		CurrController->HandlePawnDeath();
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();

		FGameplayTagContainer EffectTagsToRemove;
		EffectTagsToRemove.AddTag(EffectRemoveOnDeathTag);
		int32 NumEffectsRemoved = AbilitySystemComponent->RemoveActiveEffectsWithTags(EffectTagsToRemove);

		AbilitySystemComponent->AddLooseGameplayTag(DeadTag);
	}
	if (DeathMontage)
	{
		float AnimDuration = PlayAnimMontage(DeathMontage);
		if (GetLocalRole() < ROLE_Authority)
			return;
		FTimerHandle DeathTimer;
		GetWorldTimerManager().SetTimer(DeathTimer, this, &ABaseCharacter::FinishDying, AnimDuration - .25, false);
	}
	else
	{
		if (GetLocalRole() < ROLE_Authority)
			return;
		FinishDying();
	}
}

bool ABaseCharacter::IsAlive()
{
	if(CurrentPlayerState)
		return CurrentPlayerState->GetHealth() > 0.0f;
	return false;
}

void ABaseCharacter::FinishDying()
{
	RemoveAttachments();
	GetWorld()->DestroyActor(this);
}

void ABaseCharacter::Server_FinishDying_Implementation()
{
	FinishDying();
}

bool ABaseCharacter::Server_FinishDying_Validate()
{
	return true;
}

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(UKismetMathLibrary::GetForwardVector(GetControlRotation()), Value);
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(UKismetMathLibrary::GetRightVector(GetControlRotation()), Value);
	}
}

void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value * 45.f * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::Lookup(float Value)
{
	AddControllerPitchInput(Value * 45.f * GetWorld()->GetDeltaSeconds());
}

void ABaseCharacter::InitWeaponActor()
{
	for (FCharacterWeaponStruct CurrStruct : Addons.Weapons) {
		CurrentWeapon = GetWorld()->SpawnActor<ABaseWeaponActor>(CurrStruct.WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
		if (CurrentWeapon) {
			CurrentWeapon->SetActorOwner(this);
			CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, CurrStruct.WeaponAttachPoint);
		}
	}
}

void ABaseCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	/*
	ABasePlayerState* CurrentPlayerState = GetPlayerState<ABasePlayerState>();
	if (CurrentPlayerState)
	{
		AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(CurrentPlayerState->GetAbilitySystemComponent());

		AbilitySystemComponent->InitAbilityActorInfo(CurrentPlayerState, this);
		InitializeAttributes();

		ABasePlayerController* CurrentPlayerController = Cast<ABasePlayerController>(GetController());
		if (CurrentPlayerController)
		{
			CurrentPlayerController->InitMasterWidget();
		}
		AbilitySystemComponent->SetTagMapCount(DeadTag, 0);

		SetHealth(GetMaxHealth());
		SetStamina(GetMaxStamina());
	}
	*/
}

float ABaseCharacter::GetHealth() const
{
	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetHealth();
	}
	return 0.0f;
}

float ABaseCharacter::GetMaxHealth() const
{
	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetMaxHealth();
	}
	return 0.0f;
}

float ABaseCharacter::GetCurrentMoveSpeed() const
{
	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetMoveSpeed();
	}
	return 0.0f;
}

float ABaseCharacter::GetBaseMoveSpeed() const
{
	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetBaseMoveSpeed();
	}
	return 0.0f;
}

float ABaseCharacter::GetStamina() const
{
	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetStamina();
	}
	return 0.0f;
}

float ABaseCharacter::GetMaxStamina() const
{
	if (CurrentPlayerState)
	{
		return CurrentPlayerState->GetMaxStamina();
	}
	return 0.0f;
}

bool ABaseCharacter::ActivateAbilitiesWithTags(FGameplayTagContainer AbilityTags, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTags, bAllowRemoteActivation);
	}

	return false;
}

bool ABaseCharacter::ActivateAttackWithClass(TSubclassOf<UBaseGameplayAbility> AbilityClass, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent && bCanAttack)
	{
		return AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass, bAllowRemoteActivation);
	}

	return false;
}

bool ABaseCharacter::ForceActivateAbilityWithClass(TSubclassOf<UBaseGameplayAbility> AbilityClass, bool bAllowRemoteActivation)
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->TryActivateAbilityByClass(AbilityClass, bAllowRemoteActivation);
	}
	return false;
}
 
void ABaseCharacter::GetActiveAbilitiesWithTags(FGameplayTagContainer AbilityTags, TArray<UBaseGameplayAbility*>& ActiveAbilities)
{
	if (AbilitySystemComponent && bCanAttack)
	{
		AbilitySystemComponent->GetActiveAbilitiesWithTags(AbilityTags, ActiveAbilities);
	}
}

bool ABaseCharacter::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float& TimeRemaining, float& CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}
	return false;
}

void ABaseCharacter::SetHealth(float InHealth)
{
	if (CurrentPlayerState)
		CurrentPlayerState->SetHealth(InHealth);
}

void ABaseCharacter::SetStamina(float InStamina)
{
	if (CurrentPlayerState)
		CurrentPlayerState->SetStamina(InStamina);
}

FCharacterCombatAbilitiesWidgetDataStruct ABaseCharacter::GetPawnAbilityInfo() const
{
	return AbilityIconInfo;
}
