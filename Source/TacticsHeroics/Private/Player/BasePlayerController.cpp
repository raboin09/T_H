#include "Player/BasePlayerController.h"
#include "AbilitySystemComponent.h"
#include "Actors/BaseCharacter.h"
#include "Game/BaseGameInstance.h"
#include "Actors/BaseGeneralPawn.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/BaseGeneralHUD.h"
#include "Blueprint/UserWidget.h"
#include "Game/BaseTeam.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Player/BasePlayerState.h"

void ABasePlayerController::InitHeroPlayer()
{
	UnPossess();
	UnbindInput();
	if (InGameCurrentWidget) {
		InGameCurrentWidget->RemoveFromViewport();
	}
	ShowHeroSelectionWidget();
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, InGameCurrentWidget, EMouseLockMode::DoNotLock);
}

void ABasePlayerController::InitGeneralPlayer()
{
	UnPossess();
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ABaseGeneralPawn* SpawnedChar = CurrentPlayerState->GetTeam()->SpawnGeneralAtLocation(FVector(1430.f, 3940.f, 20.f), FRotator::ZeroRotator, GeneralPawnClass, false);
	UnbindInput();
	Possess(SpawnedChar);
	SpawnedChar->SetPlayerOwner(this);
	SetupGeneralHUD();
	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, nullptr, EMouseLockMode::DoNotLock, false);
	bShowMouseCursor = true;
	if (InGameCurrentWidget) {
		InGameCurrentWidget->RemoveFromViewport();
	}
}

void ABasePlayerController::InitHeroMasterWidget_Implementation()
{
	bShowMouseCursor = false;
	bEnableTouchEvents = true;
	UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
}

void ABasePlayerController::InitGeneralMasterWidget_Implementation()
{

}

UBaseTeam* ABasePlayerController::GetOwnerTeam()
{
	return CurrentPlayerState->GetTeam();
}

void ABasePlayerController::UnbindInput()
{
	if (InputComponent) {
		InputComponent->AxisBindings.Empty();
		InputComponent->ClearActionBindings();
	}
}

ABaseGeneralHUD* ABasePlayerController::GetGeneralHUD()
{
	return GeneralHUD;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	UBaseGameInstance* GameInst = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
	TMap<int32, FMainMenuTeamStruct> TempMap = GameInst->GetTeamData();
	CurrentPlayerState = GetPlayerState<ABasePlayerState>();
}

void ABasePlayerController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (CurrentPlayerState)
	{
		CurrentPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(CurrentPlayerState, InPawn);
	}
	CurrentPawn = InPawn;
	EPlayerType CurrRole = GetPlayerState<ABasePlayerState>()->GetPlayerType();
	if (CurrRole == EPlayerType::GENERAL) {
		InitGeneralMasterWidget();
	}
	else {
		InitHeroMasterWidget();
	}
}

void ABasePlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ABasePlayerController::SetupGeneralHUD()
{
	GeneralHUD = Cast<ABaseGeneralHUD>(MyHUD);
}
