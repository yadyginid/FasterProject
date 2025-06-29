// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStates/FasterGameStateBase.h"

#include "Net/UnrealNetwork.h"


void AFasterGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFasterGameStateBase, bCanThrowItem);
}
