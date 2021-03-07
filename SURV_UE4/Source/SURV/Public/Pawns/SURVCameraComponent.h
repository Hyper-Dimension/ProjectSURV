// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "SURVCameraComponent.generated.h"

/**
 *
 */
UCLASS(config = Game, BlueprintType, HideCategories = Trigger, meta = (BlueprintSpawnableComponent))
class SURV_API USURVCameraComponent : public UCameraComponent
{
	GENERATED_UCLASS_BODY()

public:
	// Begin UCameraComponent interface
	void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	// End UCameraComponent interface

	/** Handle zooming in */
	void OnZoomIn();

	/** Handle zooming out */
	void OnZoomOut();

	/*
	 *	Update the mouse controlled camera movement
	 *
	 *	@param	InPlayerController	The relevant player controller
	 */
	void UpdateCameraMovement(const APlayerController* InPlayerController);

	/*
	 *	Move the camera on the forward axis
	 *
	 *	@param	Val		Amount to move
	 */
	void MoveForward(float Val);

	/*
	 *	Move the camera on the left/right axis
	 *
	 *	@param	Val		Amount to move
	 */
	void MoveRight(float Val);

	/*
	 *	Exclude an area from the mouse scroll movement update. (This will be reset at the end of each update) 
	 *
	 *	@param	InCoords
	 */
	 void AddNoScrollZone(FBox InCoords);


	/** Minimum amount of camera zoom (How close we can get the map). */
	UPROPERTY(config)
	float MinZoomLevel;

	/** Maxmum amount of camera zoom (How far we can get to the map) */
	UPROPERTY(config)
	float MaxZoomLevel;

	/** The minimum offset of the camera */
	UPROPERTY(config)
	float MinCameraOffset;

	/** The maxmum offset of the camera */
	UPROPERTY(config)
	float MaxCameraOffset;

	/** The angle to look down on the map */
	UPROPERTY(config)
	FRotator FixedCameraAngle;

	/** How fast the camera moves around when the mouse is at the edge of the screen. */
	UPROPERTY(config)
	float CameraScrollSpeed;

	/** Size of the area at the edge of the screen that will trigger camera scrolling. */
	UPROPERTY(config)
	uint32 CameraActiveBorder;

	/** Sets the desired zoom level; clamping if necessary */
	void SetZoomLevel(float NewLevel);

private:
	/** Return the pawn that owns this component */
	APawn* GetOwnerPawn();

	/** Return the player controller of the pawn that owns this component */
	APlayerController* GetPlayerController();

	/** Current amount of camera zoom. */
	float ZoomAlpha;

	/** List of zones to exclude from scrolling during the camera movementt update */
	TArray<FBox> NoScrollZones;
};
