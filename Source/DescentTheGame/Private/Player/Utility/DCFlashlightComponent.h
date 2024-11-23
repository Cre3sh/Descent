// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/SpotLightComponent.h>

#include "DCFlashlightComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UDCFlashlightComponent : public USpotLightComponent
{
	GENERATED_BODY()

public:	
	UDCFlashlightComponent();

	// Begin UActorComponent override
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// End UActorComponent override

	void RechargeBattery();

	float GetBatteryPercentage() const;

	void UseLight();
	UFUNCTION(Server, Reliable)
	void Server_UseLight();

	void SwitchLight();

	bool IsOn();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> FlashlightAudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float ConsumptionRate = 1.0f;

private:
	float BatteryPercentage = 100.0f;
};