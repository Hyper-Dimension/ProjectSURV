// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SURVInput.h"
#include "Player/SURVPlayerController.h"
#include "GameFramework/PlayerInput.h"

USURVInput::USURVInput(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	, PrevTouchState(0)
{

}

void USURVInput::UpdateDetection(float DeltaTime)
{
	UpdateGameKeys(DeltaTime);
	ProcessKeyStates(DeltaTime);
}

void USURVInput::UpdateGameKeys(float DeltaTime)
{
	ASURVPlayerController* MyController = CastChecked<ASURVPlayerController>(GetOuter());

	//	gather current states
	uint32 CurrentTouchState = 0;
	for (int32 i = 0; i < UE_ARRAY_COUNT(MyController->PlayerInput->Touches); i++)
	{
		if (MyController->PlayerInput->Touches[i].Z != 0)
		{
			CurrentTouchState |= (1<<i);
		}
	}

	//	detection
	FVector2D LocalPosition1 = FVector2D(MyController->PlayerInput->Touches[0]);

	DetectOnePointActions(CurrentTouchState & 1, PrevTouchState & 1, DeltaTime, LocalPosition1, TouchAnchors[0], Touch0DownTime);

	// save states
	PrevTouchState = CurrentTouchState;
}

void USURVInput::ProcessKeyStates(float DeltaTime)
{
	for (const FActionBinding& AB : ActionBindings)
	{
		const FSimpleKeyState* KeyState = KeyStateMap.Find(AB.Key);

		if (KeyState && KeyState->Events[AB.KeyEvent] > 0)
		{
			AB.ActionDelegate.ExecuteIfBound(KeyState->Position, KeyState->DownTime);
		}
	}

	//	update states
	for (TMap<EGameKey::Type,FSimpleKeyState>::TIterator It(KeyStateMap); It; ++It)
	{
		FSimpleKeyState* const KeyState = &It.Value();

		if (KeyState->Events[IE_Pressed])
		{
			KeyState->bDown = true;
		}
		else if (KeyState->Events[IE_Released])
		{
			KeyState->bDown = false;
		}

		FMemory::Memzero(KeyState->Events, sizeof(KeyState->Events));
	}
}

void USURVInput::DetectOnePointActions(bool bCurrentState, bool bPrevState, float DeltaTime, const FVector2D& CurrentPosition, FVector2D& AnchorPosition, float& DownTime)
{
	const float HoldTime = 0.3f;

	if (bCurrentState)
	{
		// just pressed? set anchor and zero time
		if (!bPrevState)
		{
			DownTime = 0;
			AnchorPosition = CurrentPosition;
		}

		//	hold detection
		if (DownTime + DeltaTime > HoldTime && DownTime <= HoldTime)
		{
			FSimpleKeyState& HoldState = KeyStateMap.FindOrAdd(EGameKey::Hold);
			HoldState.Events[IE_Pressed]++;
			HoldState.Position = AnchorPosition;
			HoldState.DownTime = DownTime;
		}

		DownTime += DeltaTime;
	}
	else
	{
		// just released? 
		if (bPrevState)
		{
			//	tap detection
			if (DownTime < HoldTime)
			{
				FSimpleKeyState& TapState = KeyStateMap.FindOrAdd(EGameKey::Tap);
				TapState.Events[IE_Pressed]++;
				TapState.Position = AnchorPosition;
				TapState.DownTime = DownTime;
			}
			else
			{
				FSimpleKeyState& HoldState = KeyStateMap.FindOrAdd(EGameKey::Hold);
				if (HoldState.bDown)
				{
					HoldState.Events[IE_Released]++;
					HoldState.Position = AnchorPosition;
					HoldState.DownTime = DownTime;
				}
			}
		}
	}
}
