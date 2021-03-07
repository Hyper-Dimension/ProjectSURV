// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SURVGameMode.h"

/**
 * 
 */
class FSURVHelpers
{
public:
	/** convert point in screen space to ray in world space */
	static bool DeprojectScreenToWorld(const FVector2D& ScreenPosition, class ULocalPlayer* Player, FVector& RayOrigin, FVector& RayDirection);

	/** find intersection of ray in world space with ground plane */
	static FVector IntersectRayWithPlane(const FVector& RayOrigin, const FVector& RayDirection, const FPlane& Plane);
};
