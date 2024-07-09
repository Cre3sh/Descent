// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPlayerState.h"

#include "DCPlayerCharacter.h"

void ADCPlayerState::BeginPlay()
{
	Super::BeginPlay();

	ADCPlayerCharacter* const PlayerCharacter = GetPawn<ADCPlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnPlayerDied.AddDynamic(this, &ADCPlayerState::OnPlayerDied);
	}
}

void ADCPlayerState::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ADCPlayerCharacter* const PlayerCharacter = GetPawn<ADCPlayerCharacter>();
	if (IsValid(PlayerCharacter))
	{
		PlayerCharacter->OnPlayerDied.RemoveAll(this);
	}
}

void ADCPlayerState::SetPlayerDead(const bool bIsPlayerDead)
{
	bIsDead = bIsPlayerDead;
}

bool ADCPlayerState::IsPlayerDead() const
{
	return bIsDead;
}

void ADCPlayerState::OnPlayerDied()
{
	SetPlayerDead(true);
}
