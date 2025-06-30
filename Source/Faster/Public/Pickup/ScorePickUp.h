// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScorePickUp.generated.h"

class UProjectileMovementComponent;

UCLASS()
class FASTER_API AScorePickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	AScorePickUp();
	
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ScorePickUpStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly)
	int32 Points = 1;

protected:
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
