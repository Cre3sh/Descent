// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <AIController.h>
#include <BehaviorTree/BehaviorTreeComponent.h>

#include "ApparitionAIController.generated.h"

UCLASS()
class DESCENTTHEGAME_API AApparitionAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AApparitionAIController();

	// Begin AController override
	virtual void OnPossess(APawn* pawn) override;
	// End AController override

	void SetSensedTarget(APawn* NewTarget) const;
	void SetSoundLocation(const FVector& Location) const;

	void SetPlayerCaught(APawn* PlayerPawn) const;

	UBlackboardComponent* GetBlackboardComp() const;
	TArray<AActor*> GetPatrolPoints() const;

	int32 CurrentPatrolPoint = 0;

private:
	UPROPERTY(VisibleAnywhere, Category = AI)
	FName SoundLocationKey = "SoundLocation";

	/*Blackboard keys*/
	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName PlayerKey;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	FName TargetKey = "SensedPawn";

	UPROPERTY(Transient)
	TObjectPtr<UBlackboardComponent> BlackboardComponent = nullptr;
	
	UPROPERTY(Transient)
	TObjectPtr<UBehaviorTreeComponent> BehaviorComponent = nullptr;

	UPROPERTY(Transient)
	TArray<AActor*> PatrolPoints = {};
};
