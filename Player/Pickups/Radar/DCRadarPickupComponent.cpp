// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Pickups/Radar/DCRadarPickupComponent.h"

#include <Kismet/GameplayStatics.h>

#include "AI/ApparitionCharacter.h"
#include "Components/AudioComponent.h"

// Sets default values for this component's properties
UDCRadarPickupComponent::UDCRadarPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UDCRadarPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	APawn* const OwningPawn = GetOwner<APawn>();
	if (!IsValid(OwningPawn))
	{
		return;
	}

	RadarMeshComponent = Cast<UStaticMeshComponent>(OwningPawn->AddComponentByClass(UStaticMeshComponent::StaticClass(), false, OwningPawn->GetActorTransform(), false));
	RadarAudioComponent = Cast<UAudioComponent>(OwningPawn->AddComponentByClass(UAudioComponent::StaticClass(), false, OwningPawn->GetActorTransform(), false));
	
	check(RadarMeshComponent);
	check(RadarAudioComponent);

	RadarMeshComponent->SetStaticMesh(RadarMesh);

	RadarAudioComponent->SetSound(RadarBeepSound);
	RadarAudioComponent->SetVolumeMultiplier(0.35f);
}

void UDCRadarPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
	if (Distance > MaxDistance)
	{
		return;
	}

	const float DistanceNormalized = FMath::Clamp(MaxDistance / Distance * DeltaTime, 0.0f, 0.25f);

	GaugeFill = FMath::Clamp(GaugeFill + DistanceNormalized, 0.0f, StrengthGaugeMax);

	if (GaugeFill >= StrengthGaugeMax)
	{
		if (bIsOnCooldown)
		{
			return;
		}

		bIsOnCooldown = true;

		RadarAudioComponent->SetPitchMultiplier(FMath::Clamp(7500.0f / Distance, 1.0f, 2.0f));
		RadarAudioComponent->Play();

		GaugeFill = 0.0f;

		GetWorld()->GetTimerManager().SetTimer(BeepCooldownTimerHandle, FTimerDelegate::CreateLambda([this]
		{
			bIsOnCooldown = false;
		}), 0.15f, false);
	}
}