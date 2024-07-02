// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/DCHidingLocker.h"

#include <GameFramework/CharacterMovementComponent.h>

#include "Player/DCPlayerCharacter.h"

ADCHidingLocker::ADCHidingLocker()
{
	LockerMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Locker Mesh");

	LockerMeshComponent->SetupAttachment(RootComponent);

	PlayerHoldingComponent = CreateDefaultSubobject<USceneComponent>("Player Holder");

	PlayerHoldingComponent->SetupAttachment(LockerMeshComponent);

	PlayerExitComponent = CreateDefaultSubobject<USceneComponent>("Player Exit");

	PlayerExitComponent->SetupAttachment(LockerMeshComponent);
}

void ADCHidingLocker::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	check(PlayerHoldingComponent);

	// Check for multiple people trying to interact
	if (IsBeingInteracted())
	{
		if (InteractingCharacter == GetOwner())
		{
			UWorld* const World = GetWorld();
			if (IsValid(World))
			{
				FVector PlaceLocation = PlayerExitComponent->GetComponentLocation();
				World->FindTeleportSpot(InteractingCharacter, PlaceLocation, InteractingCharacter->GetActorRotation());

				InteractingCharacter->SetActorLocation(PlaceLocation);

				InteractingCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

				SetIsBeingInteracted(false);
			}
		}

		return;
	}

	if (!IsValid(InteractingCharacter))
	{
		return;
	}

	SetIsBeingInteracted(true);
	SetOwner(InteractingCharacter);

	InteractingCharacter->StartedInteractingWithObject(InteractionObjectTag);
	InteractingCharacter->SetLastInteractedObject(this);

	InteractingCharacter->SetActorLocation(PlayerHoldingComponent->GetComponentLocation());
	InteractingCharacter->SetActorRotation(PlayerHoldingComponent->GetRelativeRotation());

	InteractingCharacter->GetCharacterMovement()->DisableMovement();
}
