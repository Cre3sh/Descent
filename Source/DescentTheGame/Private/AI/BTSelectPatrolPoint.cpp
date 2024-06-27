// Fill out your copyright notice in the Description page of Project Settings.


#include "BTSelectPatrolPoint.h"
#include "AI_PatrolPoint.h"
#include "ApparitionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"




EBTNodeResult::Type UBTSelectPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AApparitionAIController* AICon = Cast<AApparitionAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		/*Get Blackboard component*/
		UBlackboardComponent* BlackboardComp = AICon->GetBlackboardComp();

		AAI_PatrolPoint* CurrentPoint = Cast<AAI_PatrolPoint>(BlackboardComp->GetValueAsObject("LocationToGo"));

		TArray<AActor*> AvailablePatrolPoints = AICon->GetPatrolPoints();

		AAI_PatrolPoint* NextPatrolPoint = nullptr;

		if (AICon->CurrentPatrolPoint != AvailablePatrolPoints.Num() - 1)
		{
			NextPatrolPoint = Cast<AAI_PatrolPoint>(AvailablePatrolPoints[++AICon->CurrentPatrolPoint]);
		}
		else //if there aren't any more points to go to
		{
			
			NextPatrolPoint = Cast<AAI_PatrolPoint>(AvailablePatrolPoints[0]);
			AICon->CurrentPatrolPoint = 0;
		}

		BlackboardComp->SetValueAsObject("LocationToGo", NextPatrolPoint);

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
