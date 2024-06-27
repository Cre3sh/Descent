// Fill out your copyright notice in the Description page of Project Settings.

#include "ApparitionAIController.h"

#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BehaviorTree.h>
#include <Kismet/GameplayStatics.h>

#include "AI_PatrolPoint.h"
#include "ApparitionCharacter.h"

AApparitionAIController::AApparitionAIController()
{
	/*Initialise blackboard and behavior tree*/
	BehaviorComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	/*Initialise blackboard keys*/
	PlayerKey = "Target";
	LocationToGoKey = "LocationToGo";
}

UBlackboardComponent* AApparitionAIController::GetBlackboardComp() const
{
	return BlackboardComponent;
}

TArray<AActor*> AApparitionAIController::GetPatrolPoints() const
{
	return PatrolPoints;
}

void AApparitionAIController::OnPossess(APawn * pawn)
{
	Super::OnPossess(pawn);
	
	/*Get reference to character*/
	const AApparitionCharacter* const AICharacter = Cast<AApparitionCharacter>(pawn);

	check(AICharacter);

	const UBehaviorTree* const BehaviorTree = AICharacter->BehaviorTree;

	check(BehaviorTree);
	

	UBlackboardData* const BlackboardData = BehaviorTree->BlackboardAsset;
	
	if (IsValid(BlackboardData))
	{
		BlackboardComponent->InitializeBlackboard(*BlackboardData);
	}

	/*Populate patrol point array*/
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAI_PatrolPoint::StaticClass(), PatrolPoints);

	check(BehaviorComponent);

	BehaviorComponent->StartTree(*AICharacter->BehaviorTree);
}

void AApparitionAIController::SetPlayerCaught(APawn* pawn) const
{
	check(BlackboardComponent);
	
	BlackboardComponent->SetValueAsObject(PlayerKey, UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AApparitionAIController::SetSensedTarget(APawn* NewTarget) const
{
	//Set a new target to follow
	check(BlackboardComponent);

	BlackboardComponent->SetValueAsObject(TargetKey, NewTarget);
}

void AApparitionAIController::SetSoundLocation(const FVector& Location) const
{
	check(BlackboardComponent);

	BlackboardComponent->SetValueAsVector(SoundLocationKey, Location);
}




