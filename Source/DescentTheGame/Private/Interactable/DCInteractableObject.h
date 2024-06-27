// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "DCInteractableObject.generated.h"

class ADCPlayerCharacter;

UCLASS()
class ADCInteractableObject : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADCInteractableObject();

	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor override
	
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter);

	UFUNCTION(Server, Reliable)
	void Server_SetIsBeingInteracted(const bool bIsInteracted);
	void SetIsBeingInteracted(const bool bIsInteracted);

	bool IsBeingInteracted() const;

private:
	UPROPERTY(Replicated)
	bool bIsBeingInteracted = false;
};