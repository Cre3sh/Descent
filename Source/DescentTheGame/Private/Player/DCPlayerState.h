// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerState.h>

#include "DCPlayerState.generated.h"

UCLASS()
class ADCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor override

	// Sets the player's death state
	void SetPlayerDead(const bool bIsPlayerDead);

	// Returns whether the pawn associated with this player state is dead
	bool IsPlayerDead() const;

private:
	UFUNCTION()
	void OnPlayerDied();

	bool bIsDead = false;
};
