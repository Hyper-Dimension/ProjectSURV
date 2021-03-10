// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/SURVTeamInterface.h"
#include "Interfaces/SURVSelectionInterface.h"
#include "SURVCharacter.generated.h"

UCLASS(Abstract)
class SURV_API ASURVCharacter : public ACharacter, 
		public ISURVTeamInterface, public ISURVSelectionInterface
{
	GENERATED_UCLASS_BODY()

	/** Identifies if pawn is in its dying state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Health)
	uint32 bIsDying:1;

	/** Currint health of this pwan */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Health)
	float Health;

public:

	/**
	* Kills pawn
	* @param KillingDamage - Damage amount of the killing blow
	* @param DamageEvent - Damage event of the killing blow
	* @param Killer - Who killed this pawn
	* @param DamageCauser - the Actor that directly caused the damage (i.e. the Projectile that exploded, the Weapon that fired, etc)
	* @returns true if allowed
	*/
	virtual void Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	/** Take damage, handle health */
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual uint8 GetTeamNum() const override;

	/** get current health */
	UFUNCTION(BlueprintCallable, Category = Health)
	int32 GetHealth() const;

	/** get max health */
	UFUNCTION(BlueprintCallable, Category = Health)
	int32 GetMaxHealth() const;
protected:

	/** team number */
	uint8 MyTeamNum;

private:
	bool OnSelectionGained_Implementation() override;

	bool OnSelectionLost_Implementation(const FVector& NewPosition, AActor* NewActor) override;

};
