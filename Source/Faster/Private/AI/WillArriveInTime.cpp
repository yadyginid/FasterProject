// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/WillArriveInTime.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AI/AICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UWillArriveInTime::UWillArriveInTime()
{
	NodeName = "Will Arrive In Time";
}

bool UWillArriveInTime::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    const AAIController* AIController = OwnerComp.GetAIOwner();
    const AAICharacter* SelfCharacter = Cast<AAICharacter>(AIController->GetPawn());
    const UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    
    UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

    if (!SelfCharacter || !BlackboardComp || !NavSys) return false;

    const AActor* TargetToy = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
    if (!TargetToy) return false;

    const FVector TargetLocation = TargetToy->GetActorLocation();
    
    float SelfTimeToTarget = FLT_MAX;
    const float SelfSpeed = SelfCharacter->MovementSpeed;

    if (SelfSpeed > KINDA_SMALL_NUMBER)
    {
        double PathLength = 0;

        ENavigationQueryResult::Type Result = NavSys->GetPathLength(
            SelfCharacter->GetActorLocation(),
            TargetLocation,
            PathLength
        );
  
        if (Result == ENavigationQueryResult::Success && PathLength > 0)
        {
            SelfTimeToTarget = PathLength / SelfSpeed;
        }
    }
    
    TArray<AActor*> AllAICharacters;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAICharacter::StaticClass(), AllAICharacters);
    
    for (AActor* AICharacterActor : AllAICharacters)
    {
        AAICharacter* RivalCharacter = Cast<AAICharacter>(AICharacterActor);
        if (!RivalCharacter || RivalCharacter == SelfCharacter) continue;

        float RivalTimeToTarget = FLT_MAX; 
        const float RivalSpeed = RivalCharacter->MovementSpeed;

        if (RivalSpeed > KINDA_SMALL_NUMBER)
        {
            double PathLength = 0;
            const ENavigationQueryResult::Type Result = NavSys->GetPathLength(
                RivalCharacter->GetActorLocation(),
                TargetLocation,
                PathLength
            );
            if (Result == ENavigationQueryResult::Success && PathLength > 0)
            {
                RivalTimeToTarget = PathLength / RivalSpeed;
            }
        }

        UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), SelfTimeToTarget);
        if (SelfTimeToTarget < RivalTimeToTarget)
        {
            return true;
        }
    }
    
    return false;
}
