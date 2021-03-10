// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interfaces/SURVTeamInterface.h"
#include "SURVAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURV_API ASURVAIController : public AAIController, public ISURVTeamInterface
{
	GENERATED_UCLASS_BODY()
	
public:
	virtual uint8 GetTeamNum() const override;


public:
	/** master switch: allows disabling all interactions */
	void EnableLogic(bool bEnable);

	/** returns information if we have logic enabled or disabled */
	bool IsLogicEnabled() const;


protected:
	/** master switch state */
	uint8 bLogicEnabled : 1;
};
