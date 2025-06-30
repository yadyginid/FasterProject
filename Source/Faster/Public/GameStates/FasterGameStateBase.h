// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FasterGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrownEvent, AActor*, ThrownActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddScoreEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeCanThrow, bool, bCanThrow);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndMatch);

UCLASS()
class FASTER_API AFasterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing=OnRep_bCanThrowItem)
	bool bCanThrowItem = true;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game")
	float MatchEndTime = 0;
	
	FOnThrownEvent OnThrownEvent;
	FOnAddScoreEvent OnAddScoreEvent;
	FOnChangeCanThrow OnChangeCanThrow;

	UPROPERTY(BlueprintAssignable)
	FOnEndMatch OnEndMatch;

	UPROPERTY(ReplicatedUsing=OnRep_bEndMatch)
	bool bEndMatch = false;
	
	void UpdateLeader();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UFUNCTION()
	void OnRep_bCanThrowItem();

	UFUNCTION()
	void OnRep_bEndMatch();
};
