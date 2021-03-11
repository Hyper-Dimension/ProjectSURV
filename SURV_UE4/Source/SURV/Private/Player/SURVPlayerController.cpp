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
#include "UI/SURVHUD.h"
#include "SURVGameState.h"
#include "Engine/World.h"
#include "Pawns/SURVCharacter.h"
#include "SURVHelpers.h"

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

	InputComponent->BindAction("TriggerAIAction", IE_Pressed, this, &ASURVPlayerController::OnTriggerAIAction);
}

void ASURVPlayerController::UpdateRotation(float DeltaTime)
{
	FRotator ViewRotation(0,0,0);
	FRotator DeltaRot(0,0,0);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);
	}

	SetControlRotation(ViewRotation);
}

void ASURVPlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	if (!bGamePaused && PlayerInput && InputHandler && !bIgnoreInput)
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
			//	skip draw minimap only update camera movement
			if (LocalPlayer->ViewportClient != NULL)
			{
				SURVSpectatorPawn->GetSURVCameraComponent()->UpdateCameraMovement(this);
			}
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
	FVector WorldPosition(0.f);
	AActor* const HitActor = GetFriendlyTarget(ScreenPosition, WorldPosition);

	SetSelectedActor(HitActor, WorldPosition);

	if (HitActor && HitActor->GetClass()->ImplementsInterface(USURVInputInterface::StaticClass()))
	{
		ISURVInputInterface::Execute_OnInputHold(HitActor);
	}
}

void ASURVPlayerController::OnHoldReleased(const FVector2D& ScreenPosition, float DownTime)
{
	AActor* const Selected = SelectedActor.Get();
	if (Selected && Selected->GetClass()->ImplementsInterface(USURVInputInterface::StaticClass()))
	{
		ISURVInputInterface::Execute_OnInputHoldReleased(Selected, DownTime);
	}
}

void ASURVPlayerController::OnToggleInGameMenu()
{
	// UE_LOG(LogGame, Log, TEXT("Player Toggle In Game Menu"));
}


void ASURVPlayerController::OnTriggerAIAction()
{
	if (SelectedActor.IsValid())
	{
		ASURVCharacter* const Selection = Cast<ASURVCharacter>(SelectedActor.Get());
		if (Selection && Selection->GetTeamNum() == GetTeamNum())
		{
			UE_LOG(LogGame, Log, TEXT("Trigger player team action"));

			ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>(Player);
			if (LocalPlayer && LocalPlayer->ViewportClient && LocalPlayer->ViewportClient->Viewport)
			{
				FVector2D MousePosition;
				if (LocalPlayer->ViewportClient->GetMousePosition(MousePosition) == false)
				{
					return;
				}

				FHitResult Hit;
				GetHitResultAtScreenPosition(MousePosition, COLLISION_PANCAMERA, FCollisionQueryParams::DefaultQueryParam, Hit);
				if (Hit.bBlockingHit)
				{
					UE_LOG(LogGame, Log, TEXT("AI action at location: %s"), *Hit.Location.ToString());
					Selection->TriggerAIAction(Hit.Location);
				}
				//FVector RayOrigin, RayDirection;
				//FSURVHelpers::DeprojectScreenToWorld(MousePosition, LocalPlayer, RayOrigin, RayDirection);

				//UWorld* World = GetWorld();
				//if (World)
				//{
				//	FHitResult Hit;
				//	World->LineTraceSingleByChannel(Hit, RayOrigin, RayDirection * 100000.f, ECC_Visibility, FCollisionQueryParams::DefaultQueryParam);

				//}
			}
		}
	}
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
				if (ISURVSelectionInterface::Execute_OnSelectionGained(NewSelectedActor))
				{
					SelectedActor = NewSelectedActor;
				}
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

