// Fill out your copyright notice in the Description page of Project Settings.


#include "Faster/Public/Pickup/ScorePickUp.h"

#include "AI/AICharacter.h"
#include "Faster/FasterGameMode.h"
#include "GameFramework/ProjectileMovementComponent.h"

AScorePickUp::AScorePickUp()
{
	PrimaryActorTick.bCanEverTick = false;

	SetReplicates(true);
	
	ScorePickUpStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("ScorePickUpStaticMesh");
	SetRootComponent(ScorePickUpStaticMesh);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
}

void AScorePickUp::BeginPlay()
{
	Super::BeginPlay();
}

void AScorePickUp::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority())
	{
		return;
	}

	const AAICharacter* AIChar = Cast<AAICharacter>(OtherActor);
	if(!AIChar) return;
	if (const auto FasterPlayerState = AIChar->GetPlayerState(); AIChar && FasterPlayerState)
	{
		if (const auto GameMode = GetWorld()->GetAuthGameMode<AFasterGameMode>())
		{
			GameMode->ScorePickUpItemWasTaken(FasterPlayerState, Points);
		}
	}

	Destroy();
}
