// Copyright Epic Games, Inc. All Rights Reserved.

#include "ElevenBitAssignementGameMode.h"
#include "ElevenBitAssignementCharacter.h"
#include "UObject/ConstructorHelpers.h"

AElevenBitAssignementGameMode::AElevenBitAssignementGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
