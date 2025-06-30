// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FasterAIController.generated.h"


UCLASS()
class FASTER_API AFasterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AFasterAIController(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

private:
	UFUNCTION()
	void OnToyThrown(AActor* ThrownActor);

	UFUNCTION()
	void OnToyTaken();
};
