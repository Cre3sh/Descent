// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/DCFlickeringLight.h"

#include <Components/AudioComponent.h>
#include <Components/PointLightComponent.h>

ADCFlickeringLight::ADCFlickeringLight()
{
	PrimaryActorTick.bCanEverTick = false;

	LightStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));

	PointLightComponent = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));

	check(PointLightComponent);

	PointLightComponent->SetupAttachment(RootComponent);

	FlickerAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Flicker Audio"));

	check(FlickerAudioComponent);

	FlickerAudioComponent->SetupAttachment(RootComponent);

	BuzzAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Buzz Audio"));

	check(BuzzAudioComponent);

	BuzzAudioComponent->SetupAttachment(RootComponent);
}

void ADCFlickeringLight::BeginPlay()
{
	Super::BeginPlay();

	check(PointLightComponent);
	check(BuzzAudioComponent);
	check(FlickerAudioComponent);

	BuzzAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	FlickerAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ADCFlickeringLight::SetFlickerActive(const bool bShouldFlicker)
{
	const UWorld* const World = GetWorld();
	check(World);

	check(PointLightComponent);
	check(FlickerAudioComponent);
	check(BuzzAudioComponent);

	if (!bShouldFlicker)
	{
		World->GetTimerManager().ClearTimer(FlickerTimerHandle);

		PointLightComponent->SetVisibility(false);

		BuzzAudioComponent->Stop();

		bIsFlickering = false;

		return;
	}

	if (!bIsFlickering)
	{
		Flicker();
	}
}

void ADCFlickeringLight::Flicker()
{
	bLightVisible = !bLightVisible;

	const UWorld* const World = GetWorld();
	check(World);

	check(PointLightComponent);
	check(FlickerAudioComponent);
	check(BuzzAudioComponent);

	bIsFlickering = true;

	PointLightComponent->SetVisibility(!PointLightComponent->IsVisible());

	const int32 RandomIndex = FMath::RandRange(0, FlickerSounds.Num() - 1);

	if (FlickerSounds.IsValidIndex(RandomIndex))
	{
		USoundBase* const SoundToPlay = FlickerSounds[RandomIndex];

		FlickerAudioComponent->SetSound(SoundToPlay);
		FlickerAudioComponent->Play();
	}

	if (!BuzzAudioComponent->IsPlaying())
	{
		BuzzAudioComponent->Play();
	}

	const float RandomDuration = FMath::RandRange(MinFlickerRate, MaxFlickerRate);

	// Don't loop as we want to set a random duration between each flicker
	World->GetTimerManager().SetTimer(FlickerTimerHandle, this, &ADCFlickeringLight::Flicker, RandomDuration, false);
}

