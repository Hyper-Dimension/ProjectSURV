// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawn.h"
#include "SURVSpectatorPawn.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class SURV_API ASURVSpectatorPawn : public ASpectatorPawn
{
	GENERATED_UCLASS_BODY()
	
// Begin ADefaultPawn interface
	/** event call on move forward input */
	void MoveForward(float Val) override;

	/** event call on move right input */
	void MoveRight(float Val) override;

	/** add custome key bindings */
	void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;

// End ADefaultPawn interface

private:
	// The camera component for this camera
	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USURVCameraComponent* SURVCameraComponent;
public:
	
	/** Handles the mouse scrolling down */
	void OnMouseScrollUp();

	/** Handles the mouse scrolling up */
	void OnMouseScrollDown();

	/** Returns a pointer to the camera component the pawn has */
	USURVCameraComponent* GetSURVCameraComponent();
};
