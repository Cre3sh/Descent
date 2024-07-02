// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>

#include "DCRadarPickupComponent.generated.h"

UCLASS(ClassGroup=(Custom), Blueprintable)
class UDCRadarPickupComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDCRadarPickupComponent();

	// Begin UActorComponent override
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponent override

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> RadarMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> RadarBeepSound = nullptr;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> RadarMeshComponent = nullptr;

	UPROPERTY()
	TObjectPtr<UAudioComponent> RadarAudioComponent = nullptr;

	FTimerHandle BeepCooldownTimerHandle;

	float StrengthGaugeMax = 1.0f;

	float GaugeFill = 0.0f;

	float MaxDistance = 10000.0f;

	bool bIsOnCooldown = false;
};