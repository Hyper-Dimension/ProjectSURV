// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SURVAIController.h"
#include "Pawns/SURVCharacter.h"
#include "SURVTypes.h"

ASURVAIController::ASURVAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}

uint8 ASURVAIController::GetTeamNum() const
{
	ASURVCharacter* const MyCharacter = Cast<ASURVCharacter>(GetPawn());
	return MyCharacter != NULL ? MyCharacter->GetTeamNum() : ESURVTeam::Unknown;
}

void ASURVAIController::EnableLogic(bool bEnable)
{
	bLogicEnabled = bEnable;
}

bool ASURVAIController::IsLogicEnabled() const
{
	return bLogicEnabled;
}
