// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Components/DCMusicManagerComponent.h"

#include <Components/AudioComponent.h>

// Sets default values for this component's properties
UDCMusicManagerComponent::UDCMusicManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UDCMusicManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	ChaseAudioComponent = Cast<UAudioComponent>(GetOwner()->AddComponentByClass(UAudioComponent::StaticClass(), false, GetOwner()->GetActorTransform(), false));

	ChaseAudioComponent->SetSound(ChaseMusic);
}

void UDCMusicManagerComponent::StartChase() const
{
	if (ChaseAudioComponent->IsPlaying())
	{
		return;
	}

	ChaseAudioComponent->Play();
}