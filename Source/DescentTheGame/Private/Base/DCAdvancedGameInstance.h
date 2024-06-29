// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <AdvancedFriendsGameInstance.h>

#include "DCAdvancedGameInstance.generated.h"

UCLASS()
class UDCAdvancedGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	FText LocalPlayerName = FText::FromString("PlayerName");
};
