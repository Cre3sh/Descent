// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DCInteractableObject.h"

#include <GameFramework/Actor.h>

#include "Door.generated.h"

UCLASS()
class DESCENTTHEGAME_API ADoor : public ADCInteractableObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();
	
	// Begin AActor override
	virtual void Tick(float DeltaTime) override;
	// End AActor override

	// Begin ADCInteractableObject override
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter) override;
	// End ADCInteractableObject override

private:
	// TODO rework these to not be 3 different audio components. No times now gotta go fast
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> DoorOpen;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> DoorClose;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> DoorLock;

	UPROPERTY(EditAnywhere)
	bool bLocked = false;

	int Multiplier = 1;

	float CurrentYaw;
	float TargetYaw;

	bool bOpen = false;
	bool bMoving;
	
	FTimerHandle DoorTimerHandle;
};
