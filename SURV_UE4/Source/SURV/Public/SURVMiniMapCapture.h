// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "SURVMiniMapCapture.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SURV_API ASURVMiniMapCapture : public ASceneCapture2D
{
	GENERATED_UCLASS_BODY()
	
protected:
	void BeginPlay() override;

public:
	void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MiniMap)
	int32 GroundLevel;

protected:

	/** updated world bounds */
	void UpdateWorldBounds();

	UPROPERTY()
	class UTextureRenderTarget2D* MiniMapView;

	/** last known location */
	FVector CachedLocation;

	/** last known FOV */
	float CachedFOV;

	/** texture was re-sized to fit desired mini map size */
	bool bTextureChanged;
};
