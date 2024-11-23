// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Usables/DCGlowstickUsable.h"

#include "Kismet/GameplayStatics.h"

ADCGlowstickUsable::ADCGlowstickUsable()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(StaticMeshComponent);
}

void ADCGlowstickUsable::Use()
{
	UGameplayStatics::PlaySound2D(this, UseSound);
	FVector PlaceLocation = GetOwner()->GetActorLocation();
	
	GetWorld()->FindTeleportSpot(this, PlaceLocation, FRotator::ZeroRotator);
	
	SetActorLocation(PlaceLocation);

	float RandomX = FMath::RandRange(200.0f, 100.0f);
	float RandomY = FMath::RandRange(200.0f, 100.0f);
	float RandomZ = FMath::RandRange(200.0f, 100.0f);
	
	StaticMeshComponent->AddImpulse(FVector(RandomX, RandomY, RandomZ));
}