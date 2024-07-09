// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Interactable/DCInteractableObject.h>

#include "DCCommandTablet.generated.h"

class ADCSlidingDoor;

UCLASS()
class ADCCommandTablet : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	ADCCommandTablet();

	// Begin ADCInteractableObject override
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;
	// End ADCInteractableObject override

	// Begin AActor override
	virtual void BeginPlay() override;
	// End AActor override

	void OnPuzzleComplete() const;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> TabletStaticMeshComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ADCSlidingDoor> OperatedDoor = nullptr;
};