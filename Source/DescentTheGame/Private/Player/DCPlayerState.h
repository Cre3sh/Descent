// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerState.h>
#include <GameplayTagContainer.h>

#include "DCPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDCOnPlayerDied, FGameplayTag, CausingEntity);

UCLASS()
class ADCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADCPlayerState();

	// Begin AActor override
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor override

	// Begin APlayerState override
	virtual FString GetPlayerNameCustom() const override;
	// End APlayerState override

	// Sets the player's death state
	void SetPlayerDead(const bool bIsPlayerDead, FGameplayTag CausingEntity);

	// Returns whether the pawn associated with this player state is dead
	bool IsPlayerDead() const;

	FDCOnPlayerDied OnPlayerDied;

private:
	UFUNCTION()
	void OnRep_PlayerDied(bool bOldDeathState);
	UPROPERTY(ReplicatedUsing=OnRep_PlayerDied)
	bool bIsDead = false;

	UPROPERTY(Replicated)
	FGameplayTag KillingEntity;
};
