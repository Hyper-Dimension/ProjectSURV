// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SURV.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/SURVTeamInterface.h"
#include "SURVPlayerController.generated.h"

class ASURVSpectatorPawn;
class USURVCameraComponent;

/**
 * 
 */
UCLASS()
class SURV_API ASURVPlayerController : public APlayerController, public ISURVTeamInterface
{
	GENERATED_UCLASS_BODY()
	
public:
	/** fixed rotation */
	void UpdateRotation(float DeltaTime) override;

protected:
	/** update input detection */
	void ProcessPlayerInput(const float DeltaTime, const bool bGamePaused) override;
	void SetupInputComponent() override;

public:
	uint8 GetTeamNum() const override;

	/** helper function to toggle input detection. */
	void SetIgnoreInput(bool bIgnore);

	/** Input handlers. */
	void OnTapPressed(const FVector2D& ScreenPosition, float DownTime);
	void OnHoldPressed(const FVector2D& ScreenPosition, float DownTime);
	void OnHoldReleased(const FVector2D& ScreenPosition, float DownTime);

	/** Toggles the ingame menu display. */
	void OnToggleInGameMenu();

	/** Trigger AI's main action, i.e. MoveTo, CancelAttack, etc..*/
	void OnTriggerAIAction();
protected:
	/** if set, input and camera updates will be ignored */
	uint8 bIgnoreInput : 1;

	/** currently selected actor */
	TWeakObjectPtr<AActor> SelectedActor;

	/** Custom input handler. */
	UPROPERTY()
	class USURVInput* InputHandler;

	/**
	 * Change current selection (on toggle on the same).
	 *
	 * @param	NewFocus	Actor to focus on.
	 * @param	NewPosition
	 */
	void SetSelectedActor(AActor* NewFocus, const FVector& NewPosition);

	/**
	 * Get friendly target under screen space coordinates.
	 *
	 * @param	ScreenPoint	Screen coordinates to check
	 * @param	WorldPoint	Point in the world the screen coordinates projected onto.
	 */
	AActor* GetFriendlyTarget(const FVector2D& ScreenPoint, FVector& WorldPoint) const;

private:
	/** Helper to return cast version of Spectator pawn. */
	ASURVSpectatorPawn* GetSURVSpectatorPawn() const;

	/** Helper to return camera component via spectator pawn */
	USURVCameraComponent* GetCameraComponent() const;
	
};
