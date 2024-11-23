// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/SaveGame.h>
#include <GameplayTagContainer.h>

#include "DCGameSave.generated.h"

UCLASS()
class UDCGameSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (Categories = "Level"))
	TMap<FGameplayTag, int32> KeyInventory;
};