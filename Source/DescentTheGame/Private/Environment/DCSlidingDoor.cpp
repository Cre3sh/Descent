// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/DCSlidingDoor.h"

#include <Components/BoxComponent.h>
#include <Components/AudioComponent.h>
#include <Net/UnrealNetwork.h>
#include <Net/Core/PushModel/PushModel.h>

ADCSlidingDoor::ADCSlidingDoor()
{
	PrimaryActorTick.bCanEverTick = true;

	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh Component"));
	SetRootComponent(DoorMeshComponent);
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));

	AccessAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Access Audio Component"));
	AccessAudioComponent->SetupAttachment(RootComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
}

void ADCSlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
	check(BoxComponent);
	check(AudioComponent);
	
	if (bShouldStartLocked)
	{
		bIsLocked = bShouldStartLocked;
	}

	BoxComponent->SetWorldLocation(GetActorLocation());
	BoxComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADCSlidingDoor::OnComponentBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADCSlidingDoor::OnComponentEndOverlap);

	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CurrentLocation = DoorMeshComponent->GetRelativeLocation();
	DoorClosePosition = CurrentLocation;
	
	DoorOpenPosition = FVector(CurrentLocation.X, CurrentLocation.Y, CurrentLocation.Z + DoorOpenZ);
}

void ADCSlidingDoor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsLocked)
	{
		return;
	}

	if (bWantsToOpen)
	{
		CurrentLocation = FMath::VInterpConstantTo(CurrentLocation, DoorOpenPosition, DeltaSeconds, 200.0f);
		DoorMeshComponent->SetRelativeLocation(CurrentLocation);

		if (CurrentLocation == DoorOpenPosition)
		{
			SetWantsToOpen(false);

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
			SetWantsToClose(false);
		}
	}
}

void ADCSlidingDoor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	check(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.RemoveAll(this);
}

void ADCSlidingDoor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ADCSlidingDoor, bWantsToOpen, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ADCSlidingDoor, bWantsToClose, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ADCSlidingDoor, bIsLocked, Params);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ADCSlidingDoor::UnlockDoor()
{
	if (!HasAuthority())
	{
		Server_UnlockDoor();
	}

	bIsLocked = false;
	AccessAudioComponent->Play();
	OpenDoor();

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCSlidingDoor, bIsLocked, this);
}

void ADCSlidingDoor::Server_UnlockDoor_Implementation()
{
	UnlockDoor();
}

void ADCSlidingDoor::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* const HitPawn = Cast<APawn>(OtherActor);
	if (!IsValid(HitPawn))
	{
		return;
	}

	if (OverlappedWeakPawns.IsEmpty())
	{
		OpenDoor();
	}

	OverlappedWeakPawns.Add(HitPawn);
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

void ADCSlidingDoor::OpenDoor()
{
	if (!bWantsToOpen)
	{
		SetWantsToOpen(true);
		SetWantsToClose(false);

		if (!bIsLocked)
		{
			AudioComponent->SetSound(DoorOpenSound);
			AudioComponent->Play();
		}
	}
}

void ADCSlidingDoor::CheckShouldClose()
{
	if (OverlappedWeakPawns.IsEmpty())
	{
		OverlappedWeakPawns.Reset();

		if (bIsLocked)
		{
			return;
		}

		SetWantsToClose(true);
		if (!bWantsToOpen)
		{
			AudioComponent->SetSound(DoorCloseSound);
			AudioComponent->Play();
		}
	}
}

void ADCSlidingDoor::SetWantsToOpen(bool bInWantsToOpen)
{
	bWantsToOpen = bInWantsToOpen;

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCSlidingDoor, bWantsToOpen, this);
}

void ADCSlidingDoor::SetWantsToClose(bool bInWantsToClose)
{
	bWantsToClose= bInWantsToClose;

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCSlidingDoor, bWantsToClose, this);
}
