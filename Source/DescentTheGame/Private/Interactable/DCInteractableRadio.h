// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable/DCInteractableObject.h"

#include "DCInteractableRadio.generated.h"

UCLASS()
class ADCInteractableRadio : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	ADCInteractableRadio();

	// Begin ADCInteractableObject override
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;
	// End ADCInteractableObject override

	// Begin AActor override
	virtual void BeginPlay() override;
	// End AActor override

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;

private:
	bool bShouldBeActive = true;
};