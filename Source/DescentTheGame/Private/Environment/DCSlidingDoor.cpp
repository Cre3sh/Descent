// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/DCSlidingDoor.h"

#include <Components/BoxComponent.h>
#include <Components/AudioComponent.h>

ADCSlidingDoor::ADCSlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh Component"));
	SetRootComponent(DoorMeshComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
}

void ADCSlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	check(BoxComponent);
	check(AudioComponent);

	BoxComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADCSlidingDoor::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADCSlidingDoor::OnComponentEndOverlap);

	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CurrentLocation = GetActorLocation();
	DoorClosePosition = CurrentLocation;
	
	DoorOpenPosition = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + DoorOpenZ);
}

void ADCSlidingDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bWantsToOpen)
	{
		CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, DoorOpenPosition, DeltaSeconds, 200.0f);
		DoorMeshComponent->SetRelativeLocation(CurrentLocation);

		if (CurrentLocation == DoorOpenPosition)
		{
			bWantsToOpen = false;

			if (bWantsToClose)
			{
				AudioComponent->SetSound(DoorCloseSound);
				AudioComponent->Play();
			}
		}
	}

	else if (bWantsToClose)
	{
		CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, DoorClosePosition, DeltaSeconds, 200.0f);
		DoorMeshComponent->SetRelativeLocation(CurrentLocation);

		if (CurrentLocation == DoorClosePosition)
		{
			bWantsToClose = false;
		}
	}
}

void ADCSlidingDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	check(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.RemoveAll(this);
}

void ADCSlidingDoor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* const HitPawn = Cast<APawn>(OtherActor);
	if (!IsValid(HitPawn))
	{
		return;
	}

	OverlappedWeakPawns.Add(HitPawn);

	if (!bWantsToOpen)
	{
		bWantsToOpen = true;
		bWantsToClose = false;

		AudioComponent->SetSound(DoorOpenSound);
		AudioComponent->Play();
	}
}

void ADCSlidingDoor::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* const HitPawn = Cast<APawn>(OtherActor);
	if (!IsValid(HitPawn))
	{
		return;
	}

	if (OverlappedWeakPawns.Contains(HitPawn))
	{
		OverlappedWeakPawns.Remove(HitPawn);
	}

	CheckShouldClose();
}

void ADCSlidingDoor::CheckShouldClose()
{
	if (OverlappedWeakPawns.IsEmpty())
	{
		bWantsToClose = true;
		if (!bWantsToOpen)
		{
			AudioComponent->SetSound(DoorCloseSound);
			AudioComponent->Play();
		}
	}
}
