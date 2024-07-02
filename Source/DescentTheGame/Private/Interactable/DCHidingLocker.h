// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interactable/DCInteractableObject.h"

#include "DCHidingLocker.generated.h"

UCLASS()
class ADCHidingLocker : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	ADCHidingLocker();
	
	// Begin ADCInteractableObject override
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;
	// End ADCInteractableObject override

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> LockerMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerHoldingComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerExitComponent = nullptr;
};
