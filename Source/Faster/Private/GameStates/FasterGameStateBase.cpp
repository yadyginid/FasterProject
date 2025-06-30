// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/FasterGameStateBase.h"

#include "FasterPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"


void AFasterGameStateBase::UpdateLeader()
{
	int32 MaxScore = 0;
	for (APlayerState* PlayerState : PlayerArray)
	{
		auto FasterPlayerState = Cast<AFasterPlayerState>(PlayerState);
		if(!FasterPlayerState) return;
		
		FasterPlayerState->bWinner = false;
		if(!FasterPlayerState) continue;
		
		if(PlayerState->GetScore() >= MaxScore)
		{
			MaxScore = PlayerState->GetScore();
		}
	}

	for (APlayerState* PlayerState : PlayerArray)
	{
		auto FasterPlayerState = Cast<AFasterPlayerState>(PlayerState);
		if(!FasterPlayerState) return;
		
		if(FasterPlayerState->GetScore() >= MaxScore)
		{
			FasterPlayerState->bWinner = true;
		}
	}
}

void AFasterGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFasterGameStateBase, bCanThrowItem);
	DOREPLIFETIME(AFasterGameStateBase, MatchEndTime);
}

void AFasterGameStateBase::OnRep_bCanThrowItem()
{
	OnChangeCanThrow.Broadcast(bCanThrowItem);
}

void AFasterGameStateBase::OnRep_bEndMatch()
{
	if(bEndMatch)
	{
		OnEndMatch.Broadcast();
	}
}
