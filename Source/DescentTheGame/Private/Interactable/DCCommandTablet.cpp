// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable/DCCommandTablet.h"

#include "Environment/DCCryogenicChamber.h"
#include "Environment/DCSlidingDoor.h"
#include "Player/DCPlayerCharacter.h"
#include "UI/Scene/Interactables/CommandTablet/DCUICommandTabletScene.h"
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

	UDCUICommandTabletScene* const CommandTabletScene = Cast<UDCUICommandTabletScene>(SceneManager->OpenScene(InteractingCharacter, Tag_UI_Scene_CommandTablet));
	if (!IsValid(CommandTabletScene))
	{
		return;
	}

	CommandTabletScene->SetTablet(this);
	CommandTabletScene->SetUseCryogenics(bHasCryogenicControls, ControlledCryogenicChamber);
}

void ADCCommandTablet::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(OperatedDoor))
	{
		OperatedDoor->SetOwner(this);
	}

	if (IsValid(ControlledCryogenicChamber))
	{
		ControlledCryogenicChamber->SetOwner(this);
	}
}

void ADCCommandTablet::OnPuzzleComplete() const
{
	ADCSlidingDoor* const SlidingDoor = OperatedDoor.Get();
	if (!IsValid(SlidingDoor))
	{
		return;
	}

	SlidingDoor->UnlockDoor();
}

void ADCCommandTablet::OnCryogenicsActivated() const
{
	ADCCryogenicChamber* const CryogenicChamber = ControlledCryogenicChamber.Get();

	check(CryogenicChamber);

	CryogenicChamber->ReviveRandomPlayer();
}