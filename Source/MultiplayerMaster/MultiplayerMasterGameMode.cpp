// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerMasterGameMode.h"
#include "MultiplayerMasterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerMasterGameMode::AMultiplayerMasterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MultiplayerMaster/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
