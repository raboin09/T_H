#include "RTSHUD.h"
#include "EngineUtils.h"
#include "Actors/BaseGeneralPawn.h"
#include "Actors/BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Player/BasePlayerController.h"

void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawSelectionFrame();
	DrawHealthBars();
	DrawHoveredActorEffect();
}

void ARTSHUD::NotifyDrawFloatingCombatText(AActor* Actor, const FString& Text, const FLinearColor& Color, float Scale, float Lifetime, float RemainingLifetime, float LifetimePercentage, float SuggestedTextLeft, float SuggestedTextTop)
{
	ReceiveDrawFloatingCombatText(
		Actor,
		Text,
		Color,
		Scale,
		Lifetime,
		RemainingLifetime,
		LifetimePercentage,
		SuggestedTextLeft,
		SuggestedTextTop);
}

void ARTSHUD::NotifyDrawHoveredActorEffect(AActor* HoveredActor)
{
	ReceiveDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::NotifyDrawHealthBar(
	AActor* Actor,
	float CurrentHealth,
	float MaximumHealth,
	float HealthPercentage,
	float SuggestedHealthBarLeft,
	float SuggestedHealthBarTop,
	float SuggestedHealthBarWidth,
	float SuggestedHealthBarHeight)
{
	ReceiveDrawHealthBar(
		Actor,
		CurrentHealth,
		MaximumHealth,
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
}

void ARTSHUD::NotifyDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height)
{
	ReceiveDrawSelectionFrame(ScreenX, ScreenY, Width, Height);
}

FVector2D ARTSHUD::GetActorCenterOnScreen(AActor* Actor) const
{
	FVector ProjectedLocation = Project(Actor->GetActorLocation());
	return FVector2D(ProjectedLocation.X, ProjectedLocation.Y);
}

FVector2D ARTSHUD::GetActorSizeOnScreen(AActor* Actor) const
{
	// Get actor position projected on HUD.
	UCapsuleComponent* CapsuleComponent = Actor->FindComponentByClass<UCapsuleComponent>();

	if (!CapsuleComponent)
	{
		return FVector2D::ZeroVector;
	}

	FCollisionShape CollisionShape = CapsuleComponent->GetCollisionShape();

	FVector ActorTopPosition = Project(Actor->GetActorLocation() + (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector ActorBottomPosition = Project(Actor->GetActorLocation() - (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector ActorLeftPosition = Project(Actor->GetActorLocation() - (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));
	FVector ActorRightPosition = Project(Actor->GetActorLocation() + (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));

	float Width = FVector(ActorRightPosition - ActorLeftPosition).Size();
	float Height = FVector(ActorTopPosition - ActorBottomPosition).Size();

	return FVector2D(Width, Height);
}

void ARTSHUD::DrawSelectionFrame()
{
	// Get selection frame.
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerOwner);
	if (!PlayerController)
	{
		return;
	}	
	
	ABaseGeneralPawn* CurrPawn = Cast<ABaseGeneralPawn>(PlayerController->GetPawn());

	if (!CurrPawn) {
		return;
	}

	FIntRect SelectionFrame;

	if (!CurrPawn->GetSelectionFrame(SelectionFrame))
	{
		return;
	}

	// Draw selection frame.
	NotifyDrawSelectionFrame(
		SelectionFrame.Min.X,
		SelectionFrame.Min.Y,
		SelectionFrame.Width(),
		SelectionFrame.Height());
}

void ARTSHUD::DrawHealthBars()
{
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerOwner);
	ABaseGeneralPawn* CurrPawn = Cast<ABaseGeneralPawn>(PlayerController->GetPawn());
	if (!PlayerController || CurrPawn)
	{
		return;
	}

	// Check override conditions.
	if (bAlwaysShowHealthBars || (bShowHotkeyHealthBars && CurrPawn->bHealthBarHotkeyPressed))
	{
		// Draw all health bars.
		for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;
			DrawHealthBar(Actor);
		}

		return;
	}

	// Draw health bar for hovered actor.
	if (bShowHoverHealthBars)
	{
		AActor* HoveredActor = CurrPawn->GetHoveredActor();
		DrawHealthBar(HoveredActor);
	}

	// Draw health bars for selected actors.
	if (bShowSelectionHealthBars)
	{
		for (int32 i = 0; i < CurrPawn->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = CurrPawn->GetSelectedActors()[i];
			DrawHealthBar(SelectedActor);
		}
	}
}

void ARTSHUD::DrawHealthBar(AActor* Actor)
{
	ABaseCharacter* CurrChar = Cast<ABaseCharacter>(Actor);
	if (!IsValid(Actor) || !CurrChar)
	{
		return;
	}

	// Check health.
	UBaseAttributeSet* HealthComponent = CurrChar->GetAttributes();

	if (!HealthComponent)
	{
		return;
	}

	const float HealthPercentage = HealthComponent->GetHealth() / HealthComponent->GetMaxHealth();

	// Suggest health bar size.
	float SuggestedHealthBarLeft;
	float SuggestedHealthBarTop;
	float SuggestedHealthBarWidth;
	float SuggestedHealthBarHeight;

	SuggestUnitBarSize(Actor, SuggestedHealthBarLeft, SuggestedHealthBarTop, SuggestedHealthBarWidth, SuggestedHealthBarHeight);

	// Draw health bar.
	NotifyDrawHealthBar(
		Actor,
		HealthComponent->GetHealth(),
		HealthComponent->GetMaxHealth(),
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
}

void ARTSHUD::DrawHoveredActorEffect()
{
	ABasePlayerController* PlayerController = Cast<ABasePlayerController>(PlayerOwner);
	ABaseGeneralPawn* CurrPawn = Cast<ABaseGeneralPawn>(PlayerController->GetPawn());
	if (!PlayerController)
	{
		return;
	}

	AActor* HoveredActor = CurrPawn->GetHoveredActor();

	if (HoveredActor == nullptr)
	{
		return;
	}

	// Draw hovered actor effect.
	NotifyDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::SuggestUnitBarSize(AActor* Actor, float& OutLeft, float& OutTop, float& OutWidth, float& OutHeight) const
{
	FVector2D Center = GetActorCenterOnScreen(Actor);
	FVector2D Size = GetActorSizeOnScreen(Actor);

	OutWidth = Size.X * 2;
	OutHeight = 15.0f;

	OutLeft = Center.X - (OutWidth / 2);
	OutTop = Center.Y - (Size.Y / 2) - (OutHeight * 2);
}