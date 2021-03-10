// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "SURVGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SURV_API ASURVGameMode : public AGameMode
{
	GENERATED_UCLASS_BODY()
public:

	/**
	 * Helper function to test teams (null = not in friendly team).
	 *
	 * @param ActorA		First actor to test against
	 * @param ActorB		Second actor to test against
	 *
	 * @return true if the two actors are on the same team.
	 */
	static bool OnFriendlyTeam(const AActor* ActorA, const AActor* ActorB);

	/**
	 * Helper function to test teams (null = not in friendly team).
	 *
	 * @param ActorA		First actor to test against
	 * @param ActorB		Second actor to test against
	 *
	 * @return true if the two actors are on opposing teams.
	 */
	static bool OnEnemyTeam(const AActor* ActorA, const AActor* ActorB);

	/**
	 *	Handle new player, skips pawn spawning.
	 *	@param	NewPlayer
	 */
	void RestartPlayer(AController* NewPlayer) override;

	/**
	 *	Modify the damage we want to apply to an actor
	 * 
	 *	@param Damage			The damage
	 *	@param DamageActor		The actor we wish to damage
	 *	@param DamageEvent		The event that caused the damage
	 *	@param EventInstigator	
	 *	@param DamageCauser
	 * 
	 *  @returns The adjusted damage amount
	 */
	 virtual float ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const DamageEvent, AController* EventInstigator, AActor* DamageCauser) const;

protected:

};
