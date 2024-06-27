// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/GameMode.h>

#include "DCGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDCOnPlayerJoinedGame, APlayerState*, PlayerState);

UCLASS()
class ADCGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Begin AGameModeBase override
	void OnPlayerJoined(AController* NewPlayer);
	// End AGameModeBase override

	FDCOnPlayerJoinedGame OnPlayerJoinedGame;
};
