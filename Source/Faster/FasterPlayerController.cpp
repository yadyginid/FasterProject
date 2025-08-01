// Copyright Epic Games, Inc. All Rights Reserved.


#include "FasterPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "FasterGameMode.h"
#include "Engine/LocalPlayer.h"

void AFasterPlayerController::Server_RestartGame_Implementation()
{
	if(const auto FasterGameMode = Cast<AFasterGameMode>(GetWorld()->GetAuthGameMode()))
	{
		FasterGameMode->Restart();
	}
}

void AFasterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}
