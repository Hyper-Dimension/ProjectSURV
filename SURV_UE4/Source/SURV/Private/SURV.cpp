// Copyright Epic Games, Inc. All Rights Reserved.

#include "SURV.h"
#include "Modules/ModuleManager.h"

class FSURVModule : public FDefaultGameModuleImpl
{
	virtual void StartupModule() override
	{

	}

	virtual void ShutdownModule() override
	{
		
	}
};

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, SURV, "SURV" );

DEFINE_LOG_CATEGORY(LogGame)