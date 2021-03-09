// Fill out your copyright notice in the Description page of Project Settings.


#include "SURVMiniMapCapture.h"
#include "Components/SceneCaptureComponent2D.h"
#include "SURVGameState.h"
#include "Engine/World.h"
#include "Engine/TextureRenderTarget2D.h"
#include "UObject/UnrealTypePrivate.h"

ASURVMiniMapCapture::ASURVMiniMapCapture(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GetCaptureComponent2D()->bCaptureEveryFrame = false;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	RootComponent->SetWorldRotation(FRotator(-90, 0, 0));
}

void ASURVMiniMapCapture::BeginPlay()
{
	Super::BeginPlay();

	//	Ensure that rotation is correct
	RootComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ASURVGameState* const MyGameState = GetWorld()->GetGameState<ASURVGameState>();
	//	minimap camera is the World Bounds limit
	if (MyGameState != nullptr)
	{
		MyGameState->MiniMapCamera = this;
	}
	CachedFOV = GetCaptureComponent2D()->FOVAngle;
	CachedLocation = RootComponent->GetComponentLocation();
	UpdateWorldBounds();
}

void ASURVMiniMapCapture::Tick(float DeltaSeconds)
{
	if (CachedFOV != GetCaptureComponent2D()->FOVAngle || CachedLocation != RootComponent->GetComponentLocation())
	{
		CachedFOV = GetCaptureComponent2D()->FOVAngle;
		CachedLocation = RootComponent->GetComponentLocation();
		UpdateWorldBounds();
	}
}

void ASURVMiniMapCapture::UpdateWorldBounds()
{
	ASURVGameState* const MyGameState = GetWorld()->GetGameState<ASURVGameState>();
	if (MyGameState != nullptr)
	{
		TArray<FVector> Points;
		FVector const CamLocation = RootComponent->GetComponentLocation();
		float DistanceFromGround = CamLocation.Z - GroundLevel;
		float Alpha = FMath::DegreesToRadians(GetCaptureComponent2D()->FOVAngle / 2);
		float MaxVisibleDistance = (DistanceFromGround / FMath::Cos(Alpha)) * FMath::Sin(Alpha);

		Points.Add(FVector(CamLocation.X + MaxVisibleDistance, CamLocation.Y + MaxVisibleDistance, GroundLevel));
		Points.Add(FVector(CamLocation.X - MaxVisibleDistance, CamLocation.Y - MaxVisibleDistance, GroundLevel));

		MyGameState->WorldBounds = FBox(Points);
		GetCaptureComponent2D()->UpdateContent();
	}
}
