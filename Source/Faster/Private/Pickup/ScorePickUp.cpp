// Fill out your copyright notice in the Description page of Project Settings.


#include "Faster/Public/Pickup/ScorePickUp.h"

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