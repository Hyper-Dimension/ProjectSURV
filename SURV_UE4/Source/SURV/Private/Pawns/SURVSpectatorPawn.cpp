// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/SURVSpectatorPawn.h"
#include "Pawns/SURVCameraComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Pawns/SURVSpectatorPawnMovement.h"
#include "Components/SphereComponent.h"
#include "Engine/CollisionProfile.h"
#include "Components/InputComponent.h"

ASURVSpectatorPawn::ASURVSpectatorPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<USURVSpectatorPawnMovement>(Super::MovementComponentName))
{
	GetCollisionComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bAddDefaultMovementBindings = false;
	SURVCameraComponent = CreateDefaultSubobject<USURVCameraComponent>(TEXT("SURVCameraComponent"));
}

void ASURVSpectatorPawn::MoveForward(float Val)
{
	SURVCameraComponent->MoveForward(Val);
}

void ASURVSpectatorPawn::MoveRight(float Val)
{
	SURVCameraComponent->MoveRight(Val);
}

void ASURVSpectatorPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("ZoomOut", IE_Pressed, this, &ASURVSpectatorPawn::OnMouseScrollDown);
	PlayerInputComponent->BindAction("ZoomIn", IE_Pressed, this, &ASURVSpectatorPawn::OnMouseScrollUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASURVSpectatorPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASURVSpectatorPawn::MoveRight);
}

void ASURVSpectatorPawn::OnMouseScrollUp()
{
	SURVCameraComponent->OnZoomIn();
}

void ASURVSpectatorPawn::OnMouseScrollDown()
{
	SURVCameraComponent->OnZoomOut();
}

USURVCameraComponent* ASURVSpectatorPawn::GetSURVCameraComponent()
{
	check(SURVCameraComponent != NULL);
	return SURVCameraComponent;
}
