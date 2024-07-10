// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerState.h>

#include "DCPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDCOnPlayerDied);

UCLASS()
class ADCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Begin AActor override
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor override

	// Sets the player's death state
	void SetPlayerDead(const bool bIsPlayerDead);

	// Returns whether the pawn associated with this player state is dead
	bool IsPlayerDead() const;

	FDCOnPlayerDied OnPlayerDied;

private:
	UFUNCTION()
	void OnRep_PlayerDied(bool bOldDeathState);
	UPROPERTY(ReplicatedUsing=OnRep_PlayerDied)
	bool bIsDead = false;
};
