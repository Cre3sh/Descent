// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerController.h>

#include "DCPlayerController.generated.h"

class ADCPlayerCharacter;
class UVOIPTalker;

UCLASS()
class ADCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Begin APlayerController override
	virtual void OnPossess(APawn* InPawn) override;
	// End APlayerController override
	
	void SpectatePlayer(ADCPlayerCharacter* PlayerCharacter);

	UFUNCTION(Server, Reliable)
	void Server_DestroyOwningPawn();
	void DestroyOwningPawn();

private:
	UPROPERTY()
	TObjectPtr<UVOIPTalker> VoipTalkerComponent = nullptr;
};
