// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "DCPickupsData.generated.h"

UCLASS()
class UDCPickupsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UActorComponent>> PickupTypes;
};