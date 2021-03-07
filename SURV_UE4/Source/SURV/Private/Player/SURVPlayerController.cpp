// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SURVPlayerController.h"
#include "SURVTypes.h"
#include "Player/SURVCheatManager.h"
#include "Player/SURVInput.h"
#include "Interfaces/SURVSelectionInterface.h"
#include "SURVGameMode.h"
#include "Interfaces/SURVInputInterface.h"
#include "Pawns/SURVSpectatorPawn.h"
#include "Pawns/SURVCameraComponent.h"
#include "Engine/LocalPlayer.h"

ASURVPlayerController::ASURVPlayerController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, bIgnoreInput(false)
{
	CheatClass = USURVCheatManager::StaticClass();
	PrimaryActorTick.bCanEverTick = true;
	SetHidden(false);
	bShowMouseCursor = true;
}

void ASURVPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputHandler = NewObject<USURVInput>(this);

	BIND_PLAYER_ACTION(InputHandler, EGameKey::Tap, IE_Pressed, &ASURVPlayerController::OnTapPressed);
	BIND_PLAYER_ACTION(InputHandler, EGameKey::Hold, IE_Pressed, &ASURVPlayerController::OnHoldPressed);
	BIND_PLAYER_ACTION(InputHandler, EGameKey::Hold, IE_Released, &ASURVPlayerController::OnHoldReleased);

	FInputActionBinding& ToggleInGameMenuBinding = InputComponent->BindAction("InGameMenu", IE_Pressed, this, &ASURVPlayerController::OnToggleInGameMenu);
	ToggleInGameMenuBinding.bExecuteWhenPaused = true;
}

void ASURVPlayerController::UpdateRotation(float DeltaTime)
{
	Super::UpdateRotation(DeltaTime);

}

void ASURVPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	if (bGamePaused && PlayerInput && InputHandler && !bIgnoreInput)
	{
		InputHandler->UpdateDetection(DeltaTime);
	}

	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
	
	if (!bIgnoreInput)
	{
		const ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
		ASURVSpectatorPawn* SURVSpectatorPawn = GetSURVSpectatorPawn();
		if (SURVSpectatorPawn != NULL && LocalPlayer != NULL)
		{
			//	TODO: Here is no minimap for SURV, you can get it back from StrategyGame
			//	only update camera movement
			SURVSpectatorPawn->GetSURVCameraComponent()->UpdateCameraMovement(this);
		}
	}
}

uint8 ASURVPlayerController::GetTeamNum() const
{
	return ESURVTeam::Player;
}

void ASURVPlayerController::SetIgnoreInput(bool bIgnore)
{
	bIgnoreInput = bIgnore;
}

void ASURVPlayerController::OnTapPressed(const FVector2D& ScreenPosition, float DownTime)
{
	FVector WorldPosition(0.f);
	AActor* const HitActor = GetFriendlyTarget(ScreenPosition, WorldPosition);

	SetSelectedActor(HitActor, WorldPosition);

	if (HitActor && HitActor->GetClass()->ImplementsInterface(USURVInputInterface::StaticClass()))
	{
		ISURVInputInterface::Execute_OnInputTap(HitActor);
	}
}

void ASURVPlayerController::OnHoldPressed(const FVector2D& ScreenPosition, float DownTime)
{

}

void ASURVPlayerController::OnHoldReleased(const FVector2D& ScreenPosition, float DownTime)
{

}

void ASURVPlayerController::OnToggleInGameMenu()
{
	// UE_LOG(LogGame, Log, TEXT("Player Toggle In Game Menu"));
}

void ASURVPlayerController::SetSelectedActor(AActor* NewSelectedActor, const FVector& NewPosition)
{
	if (SelectedActor != NewSelectedActor)
	{
		// attempt to unselect current selection
		AActor* const OldSelection = SelectedActor.Get();
		if (OldSelection && OldSelection->GetClass()->ImplementsInterface(USURVSelectionInterface::StaticClass()))
		{
			if (ISURVSelectionInterface::Execute_OnSelectionLost(OldSelection, NewPosition, NewSelectedActor))
			{
				SelectedActor = NULL;
			}
		}

		if (!SelectedActor.IsValid())
		{
			//	attempt to select new selection
			if (NewSelectedActor && NewSelectedActor->GetClass()->ImplementsInterface(USURVSelectionInterface::StaticClass()))
			{
				SelectedActor = NewSelectedActor;
			}
		}
	}
}

AActor* ASURVPlayerController::GetFriendlyTarget(const FVector2D& ScreenPoint, FVector& WorldPoint) const
{
	FHitResult Hit;
	if (GetHitResultAtScreenPosition(ScreenPoint, ECC_Pawn, true, Hit))
	{
		if (!ASURVGameMode::OnEnemyTeam(Hit.GetActor(), this))
		{
			WorldPoint = Hit.ImpactPoint;
			return Hit.GetActor();
		}
	}

	return NULL;
}

ASURVSpectatorPawn* ASURVPlayerController::GetSURVSpectatorPawn() const
{
	return Cast<ASURVSpectatorPawn>(GetSpectatorPawn());
}

USURVCameraComponent* ASURVPlayerController::GetCameraComponent() const
{
	USURVCameraComponent* CameraComponent = NULL;
	if (GetSURVSpectatorPawn() != NULL)
	{
		CameraComponent = GetSURVSpectatorPawn()->GetSURVCameraComponent();
	}
	return CameraComponent;
}

