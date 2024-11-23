// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>

#include "DCProximityRadioComponent.generated.h"

UCLASS(Blueprintable)
class UDCProximityRadioComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDCProximityRadioComponent();

	// Begin UActorComponent override
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponent override

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> SoundToPlay = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ScreenGlitchSound = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> RadioAudioComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAudioComponent> ScreenGlitchAudioComponent = nullptr;
};
