// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "WillArriveInTime.generated.h"

UCLASS()
class FASTER_API UWillArriveInTime : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UWillArriveInTime();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetKey;
};
