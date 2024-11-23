// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Objectives/DCObjectiveSubsystem.h"

void UDCObjectiveSubsystem::AddObjectivesToMap(TArray<FString> Objectives)
{
	MapObjectives = Objectives;

	UpdateObjective();

	if (OnObjectiveSet.IsBound())
	{
		OnObjectiveSet.Broadcast(CurrentObjective);
	}
}

FString UDCObjectiveSubsystem::GetCurrentObjective()
{
	return CurrentObjective;
}

void UDCObjectiveSubsystem::CompleteObjective()
{
	UpdateObjective();

	if (OnObjectiveCompleted.IsBound())
	{
		OnObjectiveCompleted.Broadcast(CurrentObjective);
	}
}

bool UDCObjectiveSubsystem::ObjectivesComplete() const
{
	return bObjectivesComplete;
}

void UDCObjectiveSubsystem::UpdateObjective()
{
	if (MapObjectives.IsValidIndex(0))
	{
		CurrentObjective = MapObjectives[0];

		MapObjectives.RemoveAt(0);

		// We have no more objectives left
		if (MapObjectives.IsEmpty())
		{
			bObjectivesComplete = true;
		}
	}
}