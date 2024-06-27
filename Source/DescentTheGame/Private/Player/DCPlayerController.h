// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/PlayerController.h>

#include "DCPlayerController.generated.h"

class UVOIPTalker;

UCLASS()
class ADCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// Begin APlayerController override
	virtual void OnPossess(APawn* InPawn) override;
	// End APlayerController override

private:
	UPROPERTY()
	TObjectPtr<UVOIPTalker> VoipTalkerComponent = nullptr;
};
