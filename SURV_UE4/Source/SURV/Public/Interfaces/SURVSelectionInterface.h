// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SURVSelectionInterface.generated.h"

/**
 * 
 */
 UINTERFACE()
class SURV_API USURVSelectionInterface : public UInterface
{
	 GENERATED_UINTERFACE_BODY()
};


class SURV_API ISURVSelectionInterface
{
	GENERATED_IINTERFACE_BODY()

	/** tries to select actor */
	UFUNCTION(BlueprintNativeEvent)
	bool OnSelectionGained();

	/** tries to deselect actor */
	UFUNCTION(BlueprintNativeEvent)
	bool OnSelectionLost(const FVector& NewPosition, AActor* NewActor);
};
