// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/DCRadarItemPickup.h"

#include "Player/DCPickupManagerComponent.h"
#include "Player/DCPlayerCharacter.h"

void ADCRadarItemPickup::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	Super::Interact(InteractingCharacter);

	if (!IsValid(InteractingCharacter))
	{
		return;
	}

	SetOwner(InteractingCharacter);
	SetIsBeingInteracted(true);

	if (!InteractingCharacter->HasAuthority())
	{
		Server_GrantPickup(InteractionObjectTag, InteractingCharacter);
	}

	GrantPickup(InteractionObjectTag, InteractingCharacter);
}

void ADCRadarItemPickup::GrantPickup(FGameplayTag InTag, ADCPlayerCharacter* PlayerCharacter)
{
	if (IsValid(PlayerCharacter))
	{
		UDCPickupManagerComponent* const PickupManagerComponent = PlayerCharacter->GetPickupManagerComponent();

		check(PickupManagerComponent);

		if (!PickupManagerComponent->AddPickupFromTag(InTag))
		{
			return;
		}

		Destroy();
	}
}

void ADCRadarItemPickup::Server_GrantPickup_Implementation(FGameplayTag InTag, ADCPlayerCharacter* PlayerCharacter)
{
	GrantPickup(InTag, PlayerCharacter);
}
