// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FasterPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateWinner);

UCLASS()
class FASTER_API AFasterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Winner)
	bool bWinner = false;

	virtual void OnRep_Score();

	UPROPERTY(BlueprintAssignable)
	FUpdateScore UpdateScore;

	UPROPERTY(BlueprintAssignable)
	FUpdateWinner UpdateWinner;
	
	UFUNCTION()
	void OnRep_Winner();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
