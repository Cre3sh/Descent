// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPlayerState.h"

#include <GameplayTagContainer.h>
#include <Net/UnrealNetwork.h>
#include <Net/Core/PushModel/PushModel.h>

#include "DCPlayerCharacter.h"

ADCPlayerState::ADCPlayerState()
{
	bUseCustomPlayerNames = true;
}

void ADCPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ADCPlayerState, bIsDead, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ADCPlayerState, KillingEntity, Params);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

FString ADCPlayerState::GetPlayerNameCustom() const
{
	ADCPlayerCharacter* const PlayerCharacter = GetPawn<ADCPlayerCharacter>();
	if (!IsValid(PlayerCharacter))
	{
		return "PlayerName";
	}

	return PlayerCharacter->CustomPlayerName.ToString();
}

void ADCPlayerState::SetPlayerDead(const bool bIsPlayerDead, FGameplayTag CausingEntity)
{
	if (bIsPlayerDead == bIsDead)
	{
		return;
	}

	bIsDead = bIsPlayerDead;

	if (bIsDead && OnPlayerDied.IsBound())
	{
		OnPlayerDied.Broadcast(CausingEntity);
	}

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerState, bIsDead, this);
	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerState, KillingEntity, this);
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
		OnPlayerDied.Broadcast(KillingEntity);
	}
}
