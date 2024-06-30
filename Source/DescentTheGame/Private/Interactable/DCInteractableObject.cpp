// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/DCInteractableObject.h"

#include <Net/UnrealNetwork.h>
#include <Net/Core/PushModel/PushModel.h>

#include "Player/DCPlayerCharacter.h"

// Sets default values
ADCInteractableObject::ADCInteractableObject()
{
	bReplicates = true;
}

void ADCInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
}

void ADCInteractableObject::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	// Overriden by derived classes to implement custom behaviour
}

void ADCInteractableObject::Server_SetIsBeingInteracted_Implementation(const bool bIsInteracted)
{
	SetIsBeingInteracted(bIsInteracted);
}

void ADCInteractableObject::SetIsBeingInteracted(const bool bIsInteracted)
{
	if (!HasAuthority())
	{
		Server_SetIsBeingInteracted(bIsInteracted);
		return;
	}

	if (!bIsInteracted)
	{
		ADCPlayerCharacter* const PlayerCharacter = GetOwner<ADCPlayerCharacter>();
		if (IsValid(PlayerCharacter))
		{
			PlayerCharacter->StoppedInteractingWithObject();
		}
	
		SetOwner(nullptr);
	}

	bIsBeingInteracted = bIsInteracted;

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCInteractableObject, bIsBeingInteracted, this);
}

bool ADCInteractableObject::IsBeingInteracted() const
{
	return bIsBeingInteracted;
}

void ADCInteractableObject::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ADCInteractableObject, bIsBeingInteracted, Params);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

FGameplayTag ADCInteractableObject::GetInteractionObjectTag() const
{
	return InteractionObjectTag;
}
