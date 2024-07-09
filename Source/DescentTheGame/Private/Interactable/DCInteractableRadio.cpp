// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/DCInteractableRadio.h"

#include <Components/AudioComponent.h>

ADCInteractableRadio::ADCInteractableRadio()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMeshComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
}

void ADCInteractableRadio::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	Super::Interact(InteractingCharacter);

	bShouldBeActive = !bShouldBeActive;

	if (bShouldBeActive)
	{
		AudioComponent->Play();
	}
	else
	{
		AudioComponent->Stop();
	}
}

void ADCInteractableRadio::BeginPlay()
{
	Super::BeginPlay();

	check(AudioComponent);

	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}
