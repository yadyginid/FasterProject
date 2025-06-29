// Copyright Epic Games, Inc. All Rights Reserved.

#include "FasterGameMode.h"
#include "FasterCharacter.h"
#include "GameStates/FasterGameStateBase.h"
#include "UObject/ConstructorHelpers.h"

AFasterGameMode::AFasterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AFasterGameMode::ScorePickUpItemWasThrow()
{
	auto FasterGameStateBase = GetGameState<AFasterGameStateBase>();
	if(!FasterGameStateBase) return;

	FasterGameStateBase->bCanThrowItem = false;
}

void AFasterGameMode::ScorePickUpItemWasTake()
{
	auto FasterGameStateBase = GetGameState<AFasterGameStateBase>();
	if(!FasterGameStateBase) return;

	FasterGameStateBase->bCanThrowItem = true;
}
