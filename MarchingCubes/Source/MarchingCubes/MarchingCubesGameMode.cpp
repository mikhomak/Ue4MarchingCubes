// Copyright Epic Games, Inc. All Rights Reserved.

#include "MarchingCubesGameMode.h"
#include "MarchingCubesCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMarchingCubesGameMode::AMarchingCubesGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
