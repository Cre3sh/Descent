// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DCInteractableObject.h"

#include "LightSwitch.generated.h"

class UPointLightComponent;

UCLASS()
class DESCENTTHEGAME_API ALightSwitch : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALightSwitch();

	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPointLightComponent> Lamp;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> SwitchMeshComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> SwitchSound;

	UPROPERTY(EditAnywhere)
	bool SwitchState = true;
};

