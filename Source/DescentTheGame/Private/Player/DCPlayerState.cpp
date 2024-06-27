// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPlayerState.h"

#include "Base/DCGameMode.h"

void ADCPlayerState::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	ADCGameMode* const GameMode = World->GetAuthGameMode<ADCGameMode>();
	if (!IsValid(GameMode))
	{
		return;
	}

	GameMode->OnPlayerJoinedGame.AddDynamic(this, &ADCPlayerState::OnNewPlayerJoined);
}

void ADCPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	const UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	ADCGameMode* const GameMode = World->GetAuthGameMode<ADCGameMode>();
	if (!IsValid(GameMode))
	{
		return;
	}

	GameMode->OnPlayerJoinedGame.RemoveAll(this);
}

void ADCPlayerState::OnNewPlayerJoined(APlayerState* PlayerState)
{
	if (OnNewPlayerStateJoined.IsBound())
	{
		OnNewPlayerStateJoined.Broadcast(PlayerState);
	}
}
