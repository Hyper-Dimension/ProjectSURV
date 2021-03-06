// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SURVTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "SURVInput.generated.h"

DECLARE_DELEGATE_TwoParams(FOnePointActionSignature, const FVector2D&, float);

#define BIND_PLAYER_ACTION(Handler, ActionKey, ActionEvent, Delegate)	\
{ \
	int32 Idx = Handler->ActionBindings.AddZeroed(); \
	Handler->ActionBindings[Idx].Key = ActionKey; \
	Handler->ActionBindings[Idx].KeyEvent = ActionEvent; \
	Handler->ActionBindings[Idx].ActionDelegate.BindUObject(this, Delegate); \
}

struct FActionBinding
{
	/** key to bind it to */
	EGameKey::Type Key;

	/** Key event to bind it to, e.g. pressed, released, dblclick */
	TEnumAsByte<EInputEvent> KeyEvent;

	/** action */
	FOnePointActionSignature ActionDelegate;
};

struct FSimpleKeyState
{
	/** current events indexed with: IE_Pressed, IE_Released, IE_Repeat */
	uint8 Events[3];

	/** is it pressed? (unused in tap & hold) */
	uint8 bDown : 1;

	/** position associated with event */
	FVector2D Position;

	/** position associated with event */
	FVector2D Position2;

	/** accumulated down time */
	float DownTime;

	FSimpleKeyState()
	{
		FMemory::Memzero(this, sizeof(FSimpleKeyState));
	}
};

/**
 * 
 */
UCLASS()
class SURV_API USURVInput : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	/** bindings for custom game events */
	TArray<FActionBinding> ActionBindings;

	/** update detection */
	void UpdateDetection(float DeltaTime);

protected:
	/** game key states */
	TMap<EGameKey::Type, FSimpleKeyState> KeyStateMap;

	/** touch anchors */
	FVector2D TouchAnchors[2];

	/** how long was touch 0 pressed? */
	float Touch0DownTime;

	/** prev touch states for recognition */
	uint32 PrevTouchState;

	/** update game key recognition */
	void UpdateGameKeys(float DeltaTime);

	/** process input state and call handlers */
	void ProcessKeyStates(float DeltaTime);

	/** detect one point actions (touch and mouse) */
	void DetectOnePointActions(bool bCurrentState, bool bPrevState, float DeltaTime, const FVector2D& CurrentPosition, FVector2D& AnchorPosition, float& DownTime);
};
