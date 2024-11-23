// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPickupManagerComponent.h"

#include <GameplayTagContainer.h>

#include "Pickups/Data/DCPickupsData.h"

// Sets default values for this component's properties
UDCPickupManagerComponent::UDCPickupManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

bool UDCPickupManagerComponent::AddPickupFromTag(const FGameplayTag InTag)
{
	check(PickupsData);

	TSubclassOf<UActorComponent> PickupComponent = PickupsData->PickupTypes.FindRef(InTag);
	if (!IsValid(PickupComponent))
	{
		return false;
	}

	APawn* const OwningPawn = GetOwner<APawn>();
	if (!IsValid(OwningPawn))
	{
		return false;
	}

	if(CurrentPlayerPickups.HasTag(InTag))
	{
		return false;
	}

	OwningPawn->AddComponentByClass(PickupComponent, false, OwningPawn->GetActorTransform(), false);

	CurrentPlayerPickups.AddTag(InTag);

	return true;
}

bool UDCPickupManagerComponent::HasPickup(const FGameplayTag InTag) const
{
	return CurrentPlayerPickups.HasTag(InTag);
}
