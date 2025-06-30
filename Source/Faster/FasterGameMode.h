// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FasterGameMode.generated.h"

class AAICharacter;

USTRUCT(BlueprintType)
struct FAiConfiguration
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AIName = TEXT("Default");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseRandomSpeed = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 500.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSpeed = 400.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 800.f;
};

UCLASS(minimalapi)
class AFasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFasterGameMode();

	virtual void StartPlay() override;
	
	void ScorePickUpItemWasThrown(AActor* ThrownItem);
	void ScorePickUpItemWasTaken(APlayerState* PlayerState, int32 Score);

	UPROPERTY(EditDefaultsOnly, Category = "AI Configuration")
	TSubclassOf<AAICharacter> AICharacterClass;

	UPROPERTY(EditDefaultsOnly, Category = "AI Configuration", meta = (TitleProperty = "AIName"))
	TArray<FAiConfiguration> AIConfigurations;

	UPROPERTY(EditDefaultsOnly)
	int32 AmountAI = 5;
	
	UFUNCTION(BlueprintCallable)
	void Restart();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	float MatchDuration = 30.f;

	UPROPERTY()
	FTimerHandle MatchTimerHandle;

	void SpawnAI();
	void SetupAISpeed(FAiConfiguration& Config, AAICharacter* NewAI);

	void EndMatch();
};



