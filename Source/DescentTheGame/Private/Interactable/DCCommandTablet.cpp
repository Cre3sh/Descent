// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/DCCommandTablet.h"

#include "Player/DCPlayerCharacter.h"
#include "UI/Systems/DCUISceneManager.h"
#include "UI/Tags/DCSceneTags.h"

ADCCommandTablet::ADCCommandTablet()
{
	TabletStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
}

void ADCCommandTablet::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	// Check for multiple people trying to interact
	if (IsBeingInteracted())
	{
		return;
	}

	if (!IsValid(InteractingCharacter))
	{
		return;
	}

	SetIsBeingInteracted(true);
	SetOwner(InteractingCharacter);

	InteractingCharacter->StartedInteractingWithObject(InteractionObjectTag);
	InteractingCharacter->SetLastInteractedObject(this);
	
	if (!InteractingCharacter->IsLocallyControlled())
	{
		return;
	}

	UDCUISceneManager* const SceneManager = InteractingCharacter->GetSceneManager();

	check(SceneManager);

	SceneManager->OpenScene(InteractingCharacter, Tag_UI_Scene_CommandTablet);
}
