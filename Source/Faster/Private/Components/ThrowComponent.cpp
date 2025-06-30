// Fill out your copyright notice in the Description page of Project Settings.

#include "Faster/Public/Components/ThrowComponent.h"
#include "Faster/FasterCharacter.h"
#include "Faster/FasterGameMode.h"
#include "GameStates/FasterGameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Pickup/ScorePickUp.h"


UThrowComponent::UThrowComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UThrowComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<AFasterCharacter>(GetOwner());
	
	FasterGameMode = Cast<AFasterGameMode>(GetWorld()->GetAuthGameMode());
	FasterGameStateBase = Cast<AFasterGameStateBase>(GetWorld()->GetGameState());
	
	Equip();
}

void UThrowComponent::Equip()
{
	if(OwnerCharacter->HasAuthority())
	{
		SelectedPickupThrowClass = PickupThrowClasses[0];
	}
}

void UThrowComponent::Client_Throw()
{
	if(!CanThrow()) return;
	OnThrowItem.Broadcast();
	Server_Throw();
}

void UThrowComponent::Client_SelectNextItem()
{
	if(!CanThrow()) return;
	Server_SelectNextItem();
}

void UThrowComponent::Client_SelectPreviousItem()
{
	if(!CanThrow()) return;

	Server_SelectPreviousItem();
}

void UThrowComponent::Server_SelectPreviousItem_Implementation()
{
	if(!CanThrow()) return;

	PickupThrowClassesIndex = (PickupThrowClassesIndex - 1) % PickupThrowClasses.Num();
	if(PickupThrowClassesIndex == -1) PickupThrowClassesIndex = 0;
	SelectedPickupThrowClass = PickupThrowClasses[PickupThrowClassesIndex];
}

void UThrowComponent::Server_SelectNextItem_Implementation()
{
	if(!CanThrow()) return;
	
	PickupThrowClassesIndex = (PickupThrowClassesIndex + 1) % PickupThrowClasses.Num();
	SelectedPickupThrowClass = PickupThrowClasses[PickupThrowClassesIndex];
}

void UThrowComponent::Server_Throw_Implementation()
{
	if(!CanThrow()) return;
	
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	const AController* OwnerController = OwnerCharacter->GetController();
	if (!OwnerController) return;
    
	OwnerController->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	
	const FVector SpawnLocation = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * 100.0f);
	const FRotator SpawnRotation = PlayerViewPointRotation;
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	
	SpawnTransform.SetScale3D(FVector(5));
	auto ScorePickUp = GetWorld()->SpawnActor<AScorePickUp>(SelectedPickupThrowClass, SpawnTransform);
	
	PlayThrowAnimation();
	CountThrowAnimation++;

	FasterGameMode->ScorePickUpItemWasThrown(ScorePickUp);
}

void UThrowComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UThrowComponent, SelectedPickupThrowClass);
	DOREPLIFETIME(UThrowComponent, CountThrowAnimation);
}

bool UThrowComponent::CanThrow() const
{
	return FasterGameStateBase->bCanThrowItem;
}

void UThrowComponent::OnRep_PickupThrowClass() const
{
	OnSelectedItemChanged.Broadcast();
}

void UThrowComponent::OnRep_CountThrowAnimation() const
{
	PlayThrowAnimation();
	OnSelectedItemChanged.Broadcast();
}

void UThrowComponent::PlayThrowAnimation() const
{
	if(!OwnerCharacter) return;
	const auto AnimInstance = OwnerCharacter->GetMesh1P()->GetAnimInstance();
	if(!AnimInstance) return;

	AnimInstance->Montage_Play(ThrowAnimation);
}
