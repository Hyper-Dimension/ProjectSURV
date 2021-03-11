// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGame, Log, All)

/**
 * when you modify this, please note that this information can be saved with instances
 * also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list
 */
#define COLLISION_PANCAMERA		ECC_GameTraceChannel1
#define COLLISION_FLOOR			ECC_GameTraceChannel2

