// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerController.h>

#include "DCPlayerController.generated.h"

class ADCPlayerCharacter;

UCLASS()
class ADCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Begin AActor override
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor override

	void SpectatePlayer(ADCPlayerCharacter* PlayerCharacter);

	UFUNCTION(Server, Reliable)
	void Server_DestroyOwningPawn();
	void DestroyOwningPawn();

private:
	FTimerHandle DestroyPlayerHandle;
};