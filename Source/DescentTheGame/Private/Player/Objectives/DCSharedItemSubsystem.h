// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Subsystems/WorldSubsystem.h>
#include <GameplayTagContainer.h>

#include "DCSharedItemSubsystem.generated.h"

// The idea of this class is that whenever a player picks up an item necessary for an objective (key, tools etc.)
// these will be shared between all players, so we don't have to grant each individual player the item, they can all use
// the items from this pool
UCLASS(BlueprintType)
class UDCSharedItemSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	// Adds an item with the specified tag to the pool
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void NetMulticast_AddItem(FGameplayTag InTag);
	
	// Consumes a single instance of an item in the item map, returns true if succeeded
	UFUNCTION(BlueprintCallable)
	bool ConsumeItem(FGameplayTag InTag);

	// Returns the item map
	TMap<FGameplayTag, int32> GetItemMap();
	
	
private:
	UPROPERTY()
	TMap<FGameplayTag, int32> ItemsMap;
};