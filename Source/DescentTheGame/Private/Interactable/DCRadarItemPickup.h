// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Interactable/DCInteractableObject.h>

#include "DCRadarItemPickup.generated.h"

UCLASS()
class ADCRadarItemPickup : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	// Begin ADCInteractableObject override
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;
	// End ADCInteractableObject override

private:
	UFUNCTION(Server, Reliable)
	void Server_GrantPickup(FGameplayTag InTag, ADCPlayerCharacter* PlayerCharacter);

	void GrantPickup(FGameplayTag InTag, ADCPlayerCharacter* PlayerCharacter);
};
