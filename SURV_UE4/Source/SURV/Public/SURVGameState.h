// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SURVTypes.h"
#include "GameFramework/GameStateBase.h"
#include "SURVGameState.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class SURV_API ASURVGameState : public AGameStateBase
{
	GENERATED_UCLASS_BODY()

public:
	/** World bounds camera movement. */
	FBox WorldBounds;
	
};
