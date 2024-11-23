// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Pickups/Radio/DCProximityRadioComponent.h"

#include <Components/AudioComponent.h>
#include <Kismet/GameplayStatics.h>

#include "AI/ApparitionCharacter.h"
#include "Camera/CameraComponent.h"

UDCProximityRadioComponent::UDCProximityRadioComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDCProximityRadioComponent::BeginPlay()
{
	Super::BeginPlay();

	RadioAudioComponent = Cast<UAudioComponent>(GetOwner()->AddComponentByClass(UAudioComponent::StaticClass(), false, GetOwner()->GetActorTransform(), false));
	ScreenGlitchAudioComponent = Cast<UAudioComponent>(GetOwner()->AddComponentByClass(UAudioComponent::StaticClass(), false, GetOwner()->GetActorTransform(), false));

	USoundAttenuation* SoundAttenuation = NewObject<USoundAttenuation>();

	SoundAttenuation->Attenuation.bAttenuate = 1;
	SoundAttenuation->Attenuation.bSpatialize = 1;

	RadioAudioComponent->SetSound(SoundToPlay);
	ScreenGlitchAudioComponent->SetSound(ScreenGlitchSound);
	ScreenGlitchAudioComponent->SetVolumeMultiplier(0.0f);

	RadioAudioComponent->Play();
	ScreenGlitchAudioComponent->Play();
}

void UDCProximityRadioComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* const OwningPawn = GetOwner<APawn>();
	
	check(OwningPawn);

	if (!OwningPawn->IsLocallyControlled())
	{
		return;
	}

	// We should only have one at a time
	AApparitionCharacter* const ApparitionCharacter = Cast<AApparitionCharacter>(UGameplayStatics::GetActorOfClass(this, AApparitionCharacter::StaticClass()));

	check(ApparitionCharacter);

	const FVector OwnerLocation = OwningPawn->GetActorLocation();
	const FVector ApparitionLocation = ApparitionCharacter->GetActorLocation();

	const float Distance = FVector::Distance(OwnerLocation, ApparitionLocation);
	if (Distance > 2000.0f)
	{
		RadioAudioComponent->SetVolumeMultiplier(0.0f);
		return;
	}

	const float RadioVolumeClamped = FMath::Clamp(1000 / Distance, 0.0f, 3.0f);
	RadioAudioComponent->SetVolumeMultiplier(RadioVolumeClamped);

	UCameraComponent* const PlayerCameraComponent = OwningPawn->GetComponentByClass<UCameraComponent>();
	if (!IsValid(PlayerCameraComponent))
	{
		return;
	}

	if (Distance > 1250.0f)
	{
		ScreenGlitchAudioComponent->SetVolumeMultiplier(0.0f);
		PlayerCameraComponent->PostProcessSettings.FilmGrainIntensity = 0.0f;

		return;
	}

	const float ScreenGlitchVolumeClamped = FMath::Clamp(1250.0f / Distance, 0.0f, 4.0f);
	UE_LOG(LogTemp,Warning, TEXT("%f"), ScreenGlitchVolumeClamped);

	ScreenGlitchAudioComponent->SetVolumeMultiplier(ScreenGlitchVolumeClamped);

	PlayerCameraComponent->PostProcessSettings.FilmGrainIntensity = FMath::Clamp(2000.0f / Distance, 0.0f, 18.0f);
}
