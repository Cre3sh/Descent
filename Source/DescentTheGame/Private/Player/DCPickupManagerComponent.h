// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>
#include <GameplayTagContainer.h>

#include "DCPickupManagerComponent.generated.h"

struct FGameplayTagContainer;
class UDCPickupsData;
struct FGameplayTag;

UCLASS(ClassGroup=(Custom))
class UDCPickupManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDCPickupManagerComponent();

	// Try to add a pickup to our owner player, will return false if failed
	bool AddPickupFromTag(const FGameplayTag InTag);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDCPickupsData> PickupsData;

	FGameplayTagContainer CurrentPlayerPickups;
};
