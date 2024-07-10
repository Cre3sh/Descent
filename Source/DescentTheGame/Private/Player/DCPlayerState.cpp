// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPlayerState.h"

#include <Net/UnrealNetwork.h>
#include <Net/Core/PushModel/PushModel.h>

void ADCPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ADCPlayerState, bIsDead, Params);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ADCPlayerState::SetPlayerDead(const bool bIsPlayerDead)
{
	if (bIsPlayerDead == bIsDead)
	{
		return;
	}

	bIsDead = bIsPlayerDead;

	if (bIsDead && OnPlayerDied.IsBound())
	{
		OnPlayerDied.Broadcast();
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerState, bIsDead, this);
}

bool ADCPlayerState::IsPlayerDead() const
{
	return bIsDead;
}

void ADCPlayerState::OnRep_PlayerDied(bool bOldDeathState)
{
	if (bOldDeathState == bIsDead)
	{
		return;
	}
	
	if (bIsDead && OnPlayerDied.IsBound())
	{
		OnPlayerDied.Broadcast();
	}
}
