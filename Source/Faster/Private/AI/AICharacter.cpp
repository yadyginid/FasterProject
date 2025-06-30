// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AICharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(HasAuthority()) return;
	if(bEnableRotate && LocalPlayer)
	{
		const FVector SelfLocation = GetActorLocation();
		const FVector PlayerLocation = LocalPlayer->GetActorLocation();
		FVector DirectionToPlayer = PlayerLocation - SelfLocation;
		
		DirectionToPlayer.Z = 0;
		
		const FRotator TargetRotation = DirectionToPlayer.Rotation();
		
		const FRotator NewRotation = FMath::RInterpTo(
			GetActorRotation(),      
			TargetRotation,          
			DeltaTime,               
			1   
		);
		
		SetActorRotation(NewRotation);
	}
}

void AAICharacter::BeginPlay()
{
	Super::BeginPlay();

	LocalPlayer = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAICharacter, MovementSpeed);
}


void AAICharacter::SetMovementSpeed(float NewSpeed)
{
	MovementSpeed = NewSpeed;
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = MovementSpeed;
	}
}

