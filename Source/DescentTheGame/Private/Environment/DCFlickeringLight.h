// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "DCFlickeringLight.generated.h"

class UPointLightComponent;

UCLASS()
class ADCFlickeringLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADCFlickeringLight();

	// Begin AActor override
	virtual void BeginPlay() override;
	// End AActor override

	void SetFlickerActive(const bool bShouldFlicker);

private:
	// Flickers once then sets a timer to do it again
	void Flicker();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LightStaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UPointLightComponent> PointLightComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> FlickerAudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> BuzzAudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<USoundBase*> FlickerSounds = {};

	UPROPERTY(EditDefaultsOnly)
	float MinFlickerRate = 0.05f;

	UPROPERTY(EditDefaultsOnly)
	float MaxFlickerRate = 0.25f;

	bool bIsFlickering = false;

	bool bLightVisible = false;

	FTimerHandle FlickerTimerHandle;
};
