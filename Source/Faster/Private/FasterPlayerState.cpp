// Fill out your copyright notice in the Description page of Project Settings.


#include "FasterPlayerState.h"
#include "Net/UnrealNetwork.h"

void AFasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	UpdateScore.Broadcast();
}

void AFasterPlayerState::OnRep_Winner()
{
	UpdateWinner.Broadcast();
}

void AFasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFasterPlayerState, bWinner);
}
