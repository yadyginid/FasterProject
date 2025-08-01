// Fill out your copyright notice in the Description page of Project Settings.


#include "Faster/Public/Pickup/ScorePickUp.h"

#include "AI/AICharacter.h"
#include "Faster/FasterGameMode.h"

AScorePickUp::AScorePickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	
	ScorePickUpStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScorePickUpStaticMesh");
	SetRootComponent(ScorePickUpStaticMesh);
	ScorePickUpStaticMesh->SetSimulatePhysics(true);
}

void AScorePickUp::BeginPlay()
{
	Super::BeginPlay();
}

void AScorePickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	const AAICharacter* AIChar = Cast<AAICharacter>(OtherActor);
	if(!AIChar) return;
	AIChar->OnItemTaken();
	
	if (!HasAuthority())
	{
		return;
	}
	
	if (const auto FasterPlayerState = AIChar->GetPlayerState(); AIChar && FasterPlayerState)
	{
		if (const auto GameMode = GetWorld()->GetAuthGameMode<AFasterGameMode>())
		{
			GameMode->ScorePickUpItemWasTaken(FasterPlayerState, Points);
		}
	}

	Destroy();
}
