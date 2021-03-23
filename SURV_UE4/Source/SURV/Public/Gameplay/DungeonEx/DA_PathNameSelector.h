// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Builders/GridFlow/GridFlowSelectorLogic.h"
#include "DA_PathNameSelector.generated.h"

/**
 * 
 */
UCLASS()
class SURV_API UDA_PathNameSelector : public UGridFlowSelectorLogic
{
	GENERATED_UCLASS_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Condition")
	TArray<FString> PathNameArr;

	bool SelectNode_Implementation(UGridFlowModel* Model, UGridFlowConfig* Config, UGridFlowBuilder* Builder, UGridFlowQuery* Query, const FRandomStream& RandomStream, int32 TileX, int32 TileY, const FTransform& MarkerTransform) override;

};
