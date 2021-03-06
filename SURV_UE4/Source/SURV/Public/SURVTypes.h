// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SURVTypes.generated.h"

UENUM(BlueprintType)
namespace ESURVTeam
{
	enum Type
	{
		Unknown,
		Player,
		Enemy,
		
		MAX
	};
}

namespace EGameKey
{
	enum Type
	{
		Tap,
		Hold,
	};
}
