// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "Player/Inventory/DCUsableItem.h"
#include "DCItemsData.generated.h"

UCLASS()
class UDCItemsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<ADCUsableItem>> ItemMap;
};