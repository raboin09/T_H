#include "UI/HUD/BaseGeneralHUD.h"
#include "Player/BasePlayerController.h"
#include "Game/BaseTeam.h"
#include "Actors/BaseCharacter.h"

FVector2D ABaseGeneralHUD::Get2DMousePosition()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return FVector2D(PosX, PosY);
}

void ABaseGeneralHUD::DrawHUD()
{
	if (bStartSelecting) {

		if (FoundActors.Num() > 0) {
			for (int32 i = 0; i < FoundActors.Num(); i++) {
				if (FoundActors[i]) {
					// FoundActors[i]->SwitchOffDecal();
				}
			}
		}

		FoundActors.Empty();
		CurrentPoint = Get2DMousePosition();
		float DiffX = CurrentPoint.X - InitialPoint.X;
		float DiffY = CurrentPoint.Y - InitialPoint.Y;
		DrawRect(FLinearColor(0, 0, 1, .15f), InitialPoint.X, InitialPoint.Y, DiffX, DiffY);
		GetActorsInSelectionRectangle<ABaseCharacter>(InitialPoint, CurrentPoint, FoundActors, false, false);
		for (int32 i = 0; i < FoundActors.Num(); i++) {
			if (FoundActors[i]) {
				UBaseTeam* CurrTeam = Cast<UBaseTeam>(FoundActors[i]->GetOwnerTeam());
				UBaseTeam* ContTeam = Cast<UBaseTeam>(Cast<ABasePlayerController>(PlayerOwner)->GetOwnerTeam());
				if (CurrTeam != ContTeam) {
					FoundActors[i] = nullptr;
				}
				else {
					// FoundActors[i]->SwitchOnDecal();
				}
			}
		}
	}
}
