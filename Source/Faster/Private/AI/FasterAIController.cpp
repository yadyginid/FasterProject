// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/FasterAIController.h"

#include "AI/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameStates/FasterGameStateBase.h"


AFasterAIController::AFasterAIController(const FObjectInitializer& ObjectInitializer)
{
	bWantsPlayerState = true;
}

void AFasterAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	if (const auto FasterGameStateBase = GetWorld()->GetGameState<AFasterGameStateBase>())
	{
		FasterGameStateBase->OnThrownEvent.AddDynamic(this, &AFasterAIController::OnToyThrown);
		FasterGameStateBase->OnAddScoreEvent.AddDynamic(this, &AFasterAIController::OnToyTaken);
	}
}

void AFasterAIController::BeginPlay()
{
	Super::BeginPlay();

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}

void AFasterAIController::OnToyThrown(AActor* ThrownActor)
{
	 Blackboard->SetValueAsObject(TEXT("PickupTarget"), ThrownActor);

	auto AIPawn = Cast<AAICharacter>(GetPawn());
	if(AIPawn)
	{
		AIPawn->SetReplicates(true);
		AIPawn->bEnableRotate = false;
	}
}

void AFasterAIController::OnToyTaken()
{
	Blackboard->SetValueAsObject(TEXT("PickupTarget"), nullptr);
	
	auto AIPawn = Cast<AAICharacter>(GetPawn());
	if(AIPawn)
	{
		AIPawn->bEnableRotate = true;
	}
}
