// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpectatorPawnMovement.h"
#include "SURVSpectatorPawnMovement.generated.h"

/**
 * 
 */
UCLASS()
class SURV_API USURVSpectatorPawnMovement : public USpectatorPawnMovement
{
	GENERATED_UCLASS_BODY()
	

	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bInitialLocationSet;
};
