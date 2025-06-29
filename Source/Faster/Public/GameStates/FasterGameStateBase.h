// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FasterGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FASTER_API AFasterGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated)
	bool bCanThrowItem = true;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
