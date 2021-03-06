// Copyright Epic Games, Inc. All Rights Reserved.


#include "SURVGameMode.h"
#include "Player/SURVPlayerController.h"
#include "Pawns/SURVSpectatorPawn.h"
#include "SURVGameState.h"

ASURVGameMode::ASURVGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerControllerClass = ASURVPlayerController::StaticClass();
	SpectatorClass = ASURVSpectatorPawn::StaticClass();
	DefaultPawnClass = ASURVSpectatorPawn::StaticClass();
	GameStateClass = ASURVGameState::StaticClass();
}

bool ASURVGameMode::OnFriendlyTeam(const AActor* ActorA, const AActor* ActorB)
{
	return false;
}

bool ASURVGameMode::OnEnemyTeam(const AActor* ActorA, const AActor* ActorB)
{
	return false;
}
