// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearSoundLocationValue.h"
#include "AI_PatrolPoint.h"
#include "ApparitionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UClearSoundLocationValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AApparitionAIController* AICon = Cast<AApparitionAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		if (AICon->GetBlackboardComponent())
		{
			AICon->GetBlackboardComponent()->ClearValue("SoundLocation");
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;

}
