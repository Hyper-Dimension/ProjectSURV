// Copyright Epic Games, Inc. All Rights Reserved.


#include "SURVGameMode.h"
#include "Player/SURVPlayerController.h"
#include "Pawns/SURVSpectatorPawn.h"
#include "SURVGameState.h"
#include "UI/SURVHUD.h"

ASURVGameMode::ASURVGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PlayerControllerClass = ASURVPlayerController::StaticClass();
	SpectatorClass = ASURVSpectatorPawn::StaticClass();
	DefaultPawnClass = ASURVSpectatorPawn::StaticClass();
	GameStateClass = ASURVGameState::StaticClass();
	HUDClass = ASURVHUD::StaticClass();
}

bool ASURVGameMode::OnFriendlyTeam(const AActor* ActorA, const AActor* ActorB)
{
	return false;
}

bool ASURVGameMode::OnEnemyTeam(const AActor* ActorA, const AActor* ActorB)
{
	return false;
}

void ASURVGameMode::RestartPlayer(AController* NewPlayer)
{
	AActor* const StartSpot = FindPlayerStart(NewPlayer);
	if (StartSpot != nullptr)
	{
		//	initialize and start it up
		InitStartSpot(StartSpot, NewPlayer);

		ASURVPlayerController* const NewPC = Cast<ASURVPlayerController>(NewPlayer);
		if (NewPC != nullptr)
		{
			NewPC->SetInitialLocationAndRotation(StartSpot->GetActorLocation(), StartSpot->GetActorRotation());
		}
		else
		{
			UE_LOG(LogGame, Warning, TEXT("Player start not found, failed to restart player"));
		}
	}
}

float ASURVGameMode::ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	return Damage;
}
