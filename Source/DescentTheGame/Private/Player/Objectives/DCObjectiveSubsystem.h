// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Subsystems/WorldSubsystem.h>

#include "DCObjectiveSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDCObjectiveCompleted, FString, NewObjectiveString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDCObjectiveSet, FString, NewObjectiveString);

UCLASS()
class UDCObjectiveSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddObjectivesToMap(TArray<FString> Objectives);

	UFUNCTION(BlueprintCallable)
	FString GetCurrentObjective();

	UFUNCTION(BlueprintCallable)
	void CompleteObjective();

	bool ObjectivesComplete() const;
	
	FDCObjectiveCompleted OnObjectiveCompleted;
	FDCObjectiveSet OnObjectiveSet;

private:
	void UpdateObjective();
	
	TArray<FString> MapObjectives;

	FString CurrentObjective = "";

	bool bObjectivesComplete = false;
};