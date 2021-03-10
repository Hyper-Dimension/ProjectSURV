// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SURVTypes.h"
#include "GameFramework/GameState.h"
#include "SURVGameState.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class SURV_API ASURVGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

public:
	/** Mini map camera component, TODO no minimap only camera for surv game, you can get full back from Strategy Gmae */
	TWeakObjectPtr<class ASURVMiniMapCapture> MiniMapCamera;
	/** World bounds camera movement. */
	FBox WorldBounds;
	
};
