// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FasterGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrownEvent, AActor*, ThrownActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAddScoreEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeCanThrow, bool, bCanThrow);

UCLASS()
class FASTER_API AFasterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(ReplicatedUsing=OnRep_bCanThrowItem)
	bool bCanThrowItem = true;
	
	FOnThrownEvent OnThrownEvent;
	FOnAddScoreEvent OnAddScoreEvent;
	FOnChangeCanThrow OnChangeCanThrow;
	
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	
	UFUNCTION()
	void OnRep_bCanThrowItem();
};
