// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Interactable/DCInteractableObject.h>

#include "DCCommandTablet.generated.h"

UCLASS()
class ADCCommandTablet : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	ADCCommandTablet();

	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> TabletStaticMeshComponent = nullptr;
};
