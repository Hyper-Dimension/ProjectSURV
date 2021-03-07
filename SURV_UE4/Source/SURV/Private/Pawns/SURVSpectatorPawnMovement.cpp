// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SURVSpectatorPawnMovement.h"
#include "Player/SURVPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Pawns/SURVSpectatorPawn.h"
#include "Pawns/SURVCameraComponent.h"

USURVSpectatorPawnMovement::USURVSpectatorPawnMovement(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	,bInitialLocationSet(false)
{
	MaxSpeed = 16000.f;
	Acceleration = 5000.f;
	Deceleration = 4000.f;
}

void USURVSpectatorPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent)
	{
		return;
	}

	ASURVPlayerController* PlayerController = Cast<ASURVPlayerController>(PawnOwner->GetController());
	if (PlayerController && PlayerController->IsLocalController())
	{
		if (!bInitialLocationSet)
		{
			PawnOwner->SetActorRotation(PlayerController->GetControlRotation());
			PawnOwner->SetActorLocation(PlayerController->GetSpawnLocation());
			bInitialLocationSet = true;
		}

		FVector MyLocation = UpdatedComponent->GetComponentLocation();
		ASURVSpectatorPawn* SpectatorPawn = Cast<ASURVSpectatorPawn>(PlayerController->GetSpectatorPawn());
		if (SpectatorPawn != NULL && SpectatorPawn->GetSURVCameraComponent() != NULL)
		{
			SpectatorPawn->GetSURVCameraComponent()->ClampCameraLocation(PlayerController, MyLocation);
		}
		UpdatedComponent->SetWorldLocation(MyLocation);
	}
}
