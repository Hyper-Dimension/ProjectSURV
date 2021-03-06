// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SURVInputInterface.generated.h"

/**
 * 
 */
UINTERFACE()
class SURV_API USURVInputInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()	
};

class SURV_API ISURVInputInterface
{
	GENERATED_IINTERFACE_BODY()

	/** receive input: tap */
	UFUNCTION(BlueprintNativeEvent)
	void OnInputTap();
};