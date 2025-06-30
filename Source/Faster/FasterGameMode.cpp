// Copyright Epic Games, Inc. All Rights Reserved.

#include "FasterGameMode.h"

#include "AIController.h"
#include "FasterCharacter.h"
#include "FasterPlayerState.h"
#include "AI/AICharacter.h"
#include "GameFramework/PlayerStart.h"
#include "GameStates/FasterGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFasterGameMode::AFasterGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AFasterGameMode::StartPlay()
{
	Super::StartPlay();
	SpawnAI();
}

void AFasterGameMode::ScorePickUpItemWasThrown(AActor* ThrownItem)
{
	auto FasterGameStateBase = GetGameState<AFasterGameStateBase>();
	if(!FasterGameStateBase) return;

	FasterGameStateBase->bCanThrowItem = false;
	FasterGameStateBase->OnThrownEvent.Broadcast(ThrownItem);
}

void AFasterGameMode::ScorePickUpItemWasTaken(APlayerState* PlayerState, int32 Score)
{
	auto FasterGameStateBase = GetGameState<AFasterGameStateBase>();
	if(!FasterGameStateBase) return;

	FasterGameStateBase->bCanThrowItem = true;
	PlayerState->SetScore(PlayerState->GetScore() + Score);

	FasterGameStateBase->OnAddScoreEvent.Broadcast();
}

void AFasterGameMode::SpawnAI()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), SpawnPoints);

	int32 SpawnPointIndex = 0;
	
	for (FAiConfiguration& Config : AIConfigurations)
	{
		if (AmountAI < SpawnPointIndex)
		{
			return;
		}

		FVector SpawnLocation = SpawnPoints[SpawnPointIndex]->GetActorLocation();
		FRotator SpawnRotation = SpawnPoints[SpawnPointIndex]->GetActorRotation();

		if (AAICharacter* NewAI = GetWorld()->SpawnActor<AAICharacter>(AICharacterClass, SpawnLocation, SpawnRotation))
		{
			SetupAISpeed(Config, NewAI);
			
			if(const auto FasterPlayerState = Cast<AFasterPlayerState>(NewAI->GetPlayerState()))
			{
				FasterPlayerState->Name = Config.AIName;
			}
			
			SpawnPointIndex++;
		}
	}
}

void AFasterGameMode::SetupAISpeed(FAiConfiguration& Config, AAICharacter* NewAI)
{
	float SpeedToSet = 0.f;
	if (Config.bUseRandomSpeed)
	{
		SpeedToSet = FMath::RandRange(Config.MinSpeed, Config.MaxSpeed);
	}
	else
	{
		SpeedToSet = Config.Speed;
	}
	NewAI->SetMovementSpeed(SpeedToSet);
}

