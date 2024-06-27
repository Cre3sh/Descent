// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerState.h>

#include "DCPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDCOnNewPlayerStateJoined, APlayerState*, PlayerState);

UCLASS()
class ADCPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor override

	FDCOnNewPlayerStateJoined OnNewPlayerStateJoined;
	
private:
	UFUNCTION()
	void OnNewPlayerJoined(APlayerState* PlayerState);
};
