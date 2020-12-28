#include "BaseGeneralPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "Input/Events.h"
#include "DrawDebugHelpers.h"
#include "Landscape.h"
#include "Game/BaseTeam.h"
#include "Engine/LocalPlayer.h"
#include "API/Game/BaseTeamInterface.h"
#include "Components/RTSSelectableComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Runtime/NavigationSystem/Public/NavFilters/NavigationQueryFilter.h"
#include "UI/HUD/BaseGeneralHUD.h"
#include "Utils/BaseGeneralPawnMovementComponent.h"
#include "Player/BasePlayerController.h"
#include "AI/BaseRTSPawnController.h"
#include "Sound/SoundCue.h"

ABaseGeneralPawn::ABaseGeneralPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	PawnMovementComponent = CreateDefaultSubobject<UBaseGeneralPawnMovementComponent>(TEXT("PawnMovementComponent"));
	PawnMovementComponent->UpdatedComponent = RootComponent;
	PawnMovementComponent->MoveSpeed = 1500.f;
}

void ABaseGeneralPawn::BeginPlay()
{
	Super::BeginPlay();	
}

void ABaseGeneralPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseGeneralPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseGeneralPawn::MoveRight);

	ABasePlayerController* PlayerCon = Cast<ABasePlayerController>(GetController());

	if (PlayerCon) {
		Owner = PlayerCon;
		PlayerCon->bShowMouseCursor = true;
		PlayerCon->bEnableClickEvents = true;
		PlayerCon->bEnableMouseOverEvents = true;
	}

	// Bind actions.
	InputComponent->BindAction(TEXT("Select"), IE_Pressed, this, &ABaseGeneralPawn::StartSelectActors);
	InputComponent->BindAction(TEXT("Select"), IE_Released, this, &ABaseGeneralPawn::FinishSelectActors);
	InputComponent->BindAction(TEXT("AddSelection"), IE_Pressed, this, &ABaseGeneralPawn::StartAddSelection);
	InputComponent->BindAction(TEXT("AddSelection"), IE_Released, this, &ABaseGeneralPawn::StopAddSelection);
	InputComponent->BindAction(TEXT("ToggleSelection"), IE_Pressed, this, &ABaseGeneralPawn::StartToggleSelection);
	InputComponent->BindAction(TEXT("ToggleSelection"), IE_Released, this, &ABaseGeneralPawn::StopToggleSelection);

	InputComponent->BindAction(TEXT("IssueOrder"), IE_Released, this, &ABaseGeneralPawn::IssueOrder);
	InputComponent->BindAction(TEXT("IssueStopOrder"), IE_Released, this, &ABaseGeneralPawn::IssueStopOrder);
}

bool ABaseGeneralPawn::GetObjectsInSelectionFrame(TArray<FHitResult>& OutHitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	FIntRect SelectionFrame;

	if (!GetSelectionFrame(SelectionFrame))
	{
		return false;
	}

	if (SelectionFrame.Area() < 10)
	{
		// Selection frame too small - just consider left-click.
		return GetObjectsAtPointerPosition(OutHitResults);
	}

	// Iterate all actors.
	OutHitResults.Reset();

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		FVector2D ActorScreenPosition;

		if (UGameplayStatics::ProjectWorldToScreen(Owner, Actor->GetActorLocation(), ActorScreenPosition))
		{
			if (SelectionFrame.Contains(FIntPoint(ActorScreenPosition.X, ActorScreenPosition.Y)))
			{
				FHitResult HitResult(Actor, nullptr, Actor->GetActorLocation(), FVector());
				OutHitResults.Add(HitResult);
			}
		}
	}

	return OutHitResults.Num() > 0;
}

bool ABaseGeneralPawn::IsSelectableActor(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return false;
	}

	auto SelectableComponent = Actor->FindComponentByClass<UAbilitySystemComponent>();

	if (!SelectableComponent)
	{
		return false;
	}

	return true;
}

bool ABaseGeneralPawn::GetSelectionFrame(FIntRect& OutSelectionFrame)
{
	if (!bCreatingSelectionFrame)
	{
		return false;
	}

	// Get mouse input.
	float MouseX;
	float MouseY;

	if (!Owner->GetMousePosition(MouseX, MouseY))
	{
		return false;
	}

	float MinX = FMath::Min(SelectionFrameMouseStartPosition.X, MouseX);
	float MaxX = FMath::Max(SelectionFrameMouseStartPosition.X, MouseX);
	float MinY = FMath::Min(SelectionFrameMouseStartPosition.Y, MouseY);
	float MaxY = FMath::Max(SelectionFrameMouseStartPosition.Y, MouseY);

	OutSelectionFrame = FIntRect(FIntPoint(MinX, MinY), FIntPoint(MaxX, MaxY));

	return true;
}

bool ABaseGeneralPawn::GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults)
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Owner->Player);

	if (!LocalPlayer || !LocalPlayer->ViewportClient)
	{
		return false;
	}

	// Get mouse position.
	FVector2D MousePosition;
	if (!LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
	{
		return false;
	}

	return GetObjectsAtScreenPosition(MousePosition, OutHitResults);
}

bool ABaseGeneralPawn::GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults)
{
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(Owner, ScreenPosition, WorldOrigin, WorldDirection))
	{
		return false;
	}

	return TraceObjects(WorldOrigin, WorldDirection, OutHitResults);
}

bool ABaseGeneralPawn::TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);

	return World->LineTraceMultiByObjectType(
		OutHitResults,
		WorldOrigin,
		WorldOrigin + WorldDirection * Owner->HitResultTraceDistance,
		Params);
}

void ABaseGeneralPawn::ServerIssueStopOrder_Implementation(APawn* OrderedPawn)
{
	auto PawnController = Cast<ABaseRTSPawnController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	PawnController->IssueStopOrder();

	NotifyOnIssuedStopOrder(OrderedPawn);
}

bool ABaseGeneralPawn::ServerIssueStopOrder_Validate(APawn* OrderedPawn)
{
	return OrderedPawn->GetOwner() == Owner;
}

void ABaseGeneralPawn::ServerIssueAttackOrder_Implementation(APawn* OrderedPawn, AActor* Target)
{
	auto PawnController = Cast<ABaseRTSPawnController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue attack order.
	PawnController->IssueAttackOrder(Target);

	// Notify listeners.
	NotifyOnIssuedAttackOrder(OrderedPawn, Target);
}

bool ABaseGeneralPawn::ServerIssueAttackOrder_Validate(APawn* OrderedPawn, AActor* Target)
{
	return OrderedPawn->GetOwner() == Owner;
}

void ABaseGeneralPawn::ServerIssueMoveOrder_Implementation(APawn* OrderedPawn, const FVector& TargetLocation)
{
	auto PawnController = Cast<ABaseRTSPawnController>(OrderedPawn->GetController());

	if (!PawnController)
	{
		return;
	}

	// Issue move order.
	PawnController->IssueMoveOrder(TargetLocation);

	// Notify listeners.
	NotifyOnIssuedMoveOrder(OrderedPawn, TargetLocation);
}

bool ABaseGeneralPawn::ServerIssueMoveOrder_Validate(APawn* OrderedPawn, const FVector& TargetLocation)
{
	return OrderedPawn->GetOwner() == Owner;
}

void ABaseGeneralPawn::MoveForward(float Value)
{
	if (Value != 0.f) {
		FVector Loc = GetActorLocation();
		FVector NewVector = { 90.0, 0.0, 0.0 };
		AddMovementInput(NewVector, Value);
	}
}

void ABaseGeneralPawn::MoveRight(float Value)
{
	if (Value != 0.f) {
		FVector NewVector = { 0.0, 90.0, 0.0 };
		AddMovementInput(NewVector, Value);
	}
}

void ABaseGeneralPawn::StartSelectActors()
{
	float MouseX;
	float MouseY;

	if (Owner->GetMousePosition(MouseX, MouseY))
	{
		SelectionFrameMouseStartPosition = FVector2D(MouseX, MouseY);
		bCreatingSelectionFrame = true;
	}
}

void ABaseGeneralPawn::FinishSelectActors()
{
	TArray<FHitResult> HitResults;

	if (!GetObjectsInSelectionFrame(HitResults))
	{
		bCreatingSelectionFrame = false;
		return;
	}

	// Check results.
	TArray<AActor*> ActorsToSelect;

	if (bAddSelectionHotkeyPressed || bToggleSelectionHotkeyPressed)
	{
		ActorsToSelect = SelectedActors;
	}

	for (auto& HitResult : HitResults)
	{
		if (!IsSelectableActor(HitResult.Actor.Get()))
		{
			continue;
		}

		// Check how to apply selection.
		if (bToggleSelectionHotkeyPressed)
		{
			if (SelectedActors.Contains(HitResult.Actor))
			{
				// Deselect actor.
				ActorsToSelect.Remove(HitResult.Actor.Get());
			}
			else if (!ActorsToSelect.Contains(HitResult.Actor))
			{
				// Select actor.
				ActorsToSelect.Add(HitResult.Actor.Get());
			}
		}
		else
		{
			if (ActorsToSelect.Contains(HitResult.Actor))
			{
				continue;
			}

			// Select actor.
			ActorsToSelect.Add(HitResult.Actor.Get());

		}
	}

	SelectActors(ActorsToSelect);

	bCreatingSelectionFrame = false;
}

void ABaseGeneralPawn::StartAddSelection()
{
	bAddSelectionHotkeyPressed = true;
}

void ABaseGeneralPawn::StopAddSelection()
{
	bAddSelectionHotkeyPressed = false;
}

void ABaseGeneralPawn::StartToggleSelection()
{
	bToggleSelectionHotkeyPressed = true;
}

void ABaseGeneralPawn::StopToggleSelection()
{
	bToggleSelectionHotkeyPressed = false;
}

void ABaseGeneralPawn::IssueOrder()
{
	TArray<FHitResult> HitResults;

	if (!GetObjectsAtPointerPosition(HitResults))
	{
		return;
	}

	IssueOrderTargetingObjects(HitResults);
}

void ABaseGeneralPawn::IssueStopOrder()
{
	for (AActor* SelectedActor : SelectedActors)
	{
		// Verify pawn and owner.
		APawn* SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Send stop order to server.
		ServerIssueStopOrder(SelectedPawn);

		if (IsNetMode(NM_Client))
		{
			// Notify listeners.
			NotifyOnIssuedStopOrder(SelectedPawn);
		}
	}
}

bool ABaseGeneralPawn::IssueAttackOrder(AActor* Target)
{
	if (!Target)
	{
		return false;
	}

	if (!Target->FindComponentByClass<UAbilitySystemComponent>())
	{
		return false;
	}

	// Issue attack orders.
	bool bSuccess = false;

	for (AActor* SelectedActor : SelectedActors)
	{
		ABaseCharacter* SelectedPawn = Cast<ABaseCharacter>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Verify target.
		UAbilitySystemComponent* TargetOwnerComponent = Target->FindComponentByClass<UAbilitySystemComponent>();

		if (TargetOwnerComponent && SelectedPawn->GetOwnerTeam()->IsPawnOnThisTeam(Target))
		{
			continue;
		}

		// Send attack order to server.
		ServerIssueAttackOrder(SelectedPawn, Target);

		if (IsNetMode(NM_Client))
		{

			// Notify listeners.
			NotifyOnIssuedAttackOrder(SelectedPawn, Target);
		}

		bSuccess = true;
	}

	return bSuccess;
}

bool ABaseGeneralPawn::IssueMoveOrder(const FVector& TargetLocation)
{
	bool bSuccess = false;

	for (auto SelectedActor : SelectedActors)
	{
		// Verify pawn and owner.
		auto SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}

		if (SelectedPawn->GetOwner() != this)
		{
			continue;
		}

		// Send move order to server.
		ServerIssueMoveOrder(SelectedPawn, TargetLocation);

		if (IsNetMode(NM_Client))
		{
			// Notify listeners.
			NotifyOnIssuedMoveOrder(SelectedPawn, TargetLocation);
		}

		bSuccess = true;
	}

	return bSuccess;
}

void ABaseGeneralPawn::IssueOrderTargetingObjects(TArray<FHitResult>& HitResults)
{
	if (SelectedActors.Num() == 0)
	{
		return;
	}

	for (auto& HitResult : HitResults)
	{
		if (HitResult.Actor != nullptr)
		{
			if (IssueAttackOrder(HitResult.Actor.Get()))
			{
				return;
			}

			ALandscape* Landscape = Cast<ALandscape>(HitResult.Actor.Get());

			if (Landscape != nullptr)
			{
				IssueMoveOrder(HitResult.Location);
				return;
			}

			continue;
		}

		IssueMoveOrder(HitResult.Location);
		return;
	}
}

void ABaseGeneralPawn::SetOwnerTeam(UBaseTeam* InTeam)
{
	OwnerTeam = InTeam;
}

UPawnMovementComponent* ABaseGeneralPawn::GetMovementComponent() const
{
	return PawnMovementComponent;
}

void ABaseGeneralPawn::SetPlayerOwner(ABasePlayerController* InController)
{
	Owner = InController;
}

void ABaseGeneralPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABaseGeneralPawn::SelectActors(TArray<AActor*> Actors)
{
	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->DeselectActor();
		}
	}

	// Apply new selection.
	SelectedActors = Actors;

	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent) {
			SelectableComponent->SelectActor();
			if (SelectionSoundCooldownRemaining <= 0.0f && IsValid(SelectableComponent->SelectedSound))
			{
				UGameplayStatics::PlaySound2D(this, SelectableComponent->SelectedSound);
				SelectionSoundCooldownRemaining = SelectableComponent->SelectedSound->GetDuration();
			}
		}
	}

	NotifyOnSelectionChanged(SelectedActors);
}

TArray<AActor*> ABaseGeneralPawn::GetSelectedActors()
{
	return SelectedActors;
}

void ABaseGeneralPawn::NotifyOnActorOwnerChanged(AActor* Actor)
{

}

void ABaseGeneralPawn::NotifyOnErrorOccurred(const FString& ErrorMessage)
{

}

void ABaseGeneralPawn::NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target)
{

}

void ABaseGeneralPawn::NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation)
{

}

void ABaseGeneralPawn::NotifyOnIssuedStopOrder(APawn* OrderedPawn)
{

}

void ABaseGeneralPawn::NotifyOnMinimapClicked(const FPointerEvent& InMouseEvent, const FVector2D& MinimapPosition, const FVector& WorldPosition)
{

}

void ABaseGeneralPawn::NotifyOnSelectionChanged(const TArray<AActor*>& Selection)
{

}

AActor* ABaseGeneralPawn::GetHoveredActor()
{
	return HoveredActor;
}
