// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SURVCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/SURVAIController.h"
#include "Components/CapsuleComponent.h"
#include "SURVGameMode.h"
#include "GameFramework/DamageType.h"

ASURVCharacter::ASURVCharacter(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	//	no collision in mesh
	GetMesh()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->UpdatedComponent = GetCapsuleComponent();
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanWalk = true;
		GetCharacterMovement()->GetNavAgentPropertiesRef().bCanJump = true;
		GetCharacterMovement()->SetJumpAllowed(true);
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->bAlwaysCheckFloor = false;
	}

	Health = 100;
	AIControllerClass = ASURVAIController::StaticClass();
}

void ASURVCharacter::Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser)
{
	if (bIsDying					//	already dying
	|| IsPendingKill())				//	already destroyed
	{
		return;
	}

	bIsDying = true;
	Health = FMath::Min(0.0f, Health);

	//	figure out who killed us
	UDamageType const* const DamageType = DamageEvent.DamageTypeClass ?
		Cast<const UDamageType>(DamageEvent.DamageTypeClass.GetDefaultObject())
		: GetDefault<UDamageType>();
	Killer = GetDamageInstigator(Killer, *DamageType);


	//	TODO: maybe game state wants to know the enemy dies, get back from Strategy Game

	//	disable any AI
	ASURVAIController* const AIController = Cast<ASURVAIController>(Controller);
	if (AIController)
	{
		AIController->EnableLogic(false);
	}
}

uint8 ASURVCharacter::GetTeamNum() const
{
	return MyTeamNum;
}


void ASURVCharacter::SetTeamNum(uint8 NewTeamNum)
{
	MyTeamNum = NewTeamNum;
}

int32 ASURVCharacter::GetHealth() const
{
	return Health;
}

int32 ASURVCharacter::GetMaxHealth() const
{
	return GetClass()->GetDefaultObject<ASURVCharacter>()->GetHealth();
}


void ASURVCharacter::TriggerAIAction_Implementation(const FVector& ActionLocation)
{
	UE_LOG(LogGame, Log, TEXT("Trigger AI action from CPP"));
}

bool ASURVCharacter::OnSelectionLost_Implementation(const FVector& NewPosition, AActor* NewActor)
{
	//	TODO UNSELECTED
	return true;
}

bool ASURVCharacter::OnSelectionGained_Implementation()
{
	return true;
}

float ASURVCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health < 0.0f)
	{
		//	no further damage if already dead
		return 0.f;
	}

	//	Modify based on game rules.
	ASURVGameMode* const Game = GetWorld()->GetAuthGameMode<ASURVGameMode>();
	Damage = Game ? Game->ModifyDamage(Damage, this, DamageEvent, EventInstigator, DamageCauser) : 0.f;

	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		Health -= ActualDamage;
		if (Health <= 0.f)
		{
			Die(ActualDamage, DamageEvent, EventInstigator, DamageCauser);
		}

		//	broadcast AI-detectable noise
		MakeNoise(1.0f, EventInstigator ? EventInstigator->GetPawn() : this);

		//	send message to game state (SURV no need, get back from Strategy Game)
	}

	return ActualDamage;
}
