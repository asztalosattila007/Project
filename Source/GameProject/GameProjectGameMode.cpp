// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameProjectGameMode.h"
#include "GameProjectCharacter.h"

AGameProjectGameMode::AGameProjectGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AGameProjectCharacter::StaticClass();	
}
