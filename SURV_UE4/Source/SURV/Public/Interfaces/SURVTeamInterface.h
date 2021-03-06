// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SURVTeamInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class SURV_API USURVTeamInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()	
};

class SURV_API ISURVTeamInterface
{
	GENERATED_IINTERFACE_BODY()

	/** returns the team number of the actor */
	virtual uint8 GetTeamNum() const = 0;
};