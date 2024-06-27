// Fill out your copyright notice in the Description page of Project Settings.

#include "LightSwitch.h"

#include <Components/PointLightComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Components/AudioComponent.h>

// Sets default values
ALightSwitch::ALightSwitch()
{
	Lamp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Lamp"));
	SwitchMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Switch"));
	SwitchSound = CreateDefaultSubobject<UAudioComponent>(TEXT("Switch Sound"));
}

void ALightSwitch::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	SwitchState = !SwitchState;
	SwitchSound->Play();

	Lamp->SetVisibility(SwitchState);
}