// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Utility/DCFlashlightComponent.h"

#include <Components/AudioComponent.h>

// Sets default values for this component's properties
UDCFlashlightComponent::UDCFlashlightComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FlashlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FlashlightAudio"));
}

// Called every frame
void UDCFlashlightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BatteryPercentage <= 0.0f)
	{
		SetVisibility(false);
	}
	else if (IsVisible())
	{
		BatteryPercentage -= DeltaTime * ConsumptionRate;
	}
}

void UDCFlashlightComponent::RechargeBattery()
{
	BatteryPercentage = 100.0f;
}

float UDCFlashlightComponent::GetBatteryPercentage() const
{
	return BatteryPercentage;
}

void UDCFlashlightComponent::UseLight()
{
	if (!GetOwner()->HasAuthority())
	{
		Server_UseLight();
	}

	SwitchLight();
}

void UDCFlashlightComponent::Server_UseLight_Implementation()
{
	UseLight();
}

void UDCFlashlightComponent::SwitchLight()
{
	if (!IsVisible() && BatteryPercentage <= 0.0f)
	{
		return;
	}

	check(FlashlightAudioComponent);

	SetVisibility(!IsVisible());

	FlashlightAudioComponent->Play();
}

bool UDCFlashlightComponent::IsOn()
{
	return IsVisible();
}
