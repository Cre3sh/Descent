// Fill out your copyright notice in the Description page of Project Settings.

#include "Audio/DCTerrorRadiusComponent.h"

#include <GameFramework/GameStateBase.h>
#include <GameFramework/PlayerState.h>
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>

UDCTerrorRadiusComponent::UDCTerrorRadiusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(false);
}

void UDCTerrorRadiusComponent::BeginPlay()
{
	Super::BeginPlay();

	const UWorld* const World = GetWorld();

	check(World);

	World->GetTimerManager().SetTimer(CheckPlayerHandle, this, &UDCTerrorRadiusComponent::CheckForNearbyPlayers, 10.0f, true);
}

void UDCTerrorRadiusComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	const UWorld* const World = GetWorld();

	check(World);

	World->GetTimerManager().ClearTimer(CheckPlayerHandle);
}

void UDCTerrorRadiusComponent::OnAudioFinished()
{
	bIsPlaying = false;
}

void UDCTerrorRadiusComponent::CheckForNearbyPlayers()
{
	const UWorld* const World = GetWorld();

	check(World);

	AGameStateBase* const GameState = World->GetGameState();

	check(GameState);

	bool bHasFoundClosePlayer = false;
	bool bFarFromPlayers = false;

	float MaxDistance = 0.0f;
	for (const APlayerState* const PlayerState : GameState->PlayerArray)
	{
		if (!IsValid(PlayerState))
		{
			continue;
		}

		const APawn* const Pawn = PlayerState->GetPawn();
		if (!IsValid(Pawn))
		{
			continue;
		}

		float Distance = FVector::Distance(GetOwner()->GetActorLocation(), Pawn->GetActorLocation());

		MaxDistance = Distance > MaxDistance ? Distance : MaxDistance;

		if (MaxDistance <= 1000.0f)
		{
			bHasFoundClosePlayer = true;
			break;
		}
	}

	if (MaxDistance >= 5000.0f)
	{
		bFarFromPlayers = true;
	}

	if (IsValid(AudioComponent) && bHasFoundClosePlayer)
	{
		AudioComponent->Stop();
		bIsPlaying = false;
	}
	else if (bFarFromPlayers && !bIsPlaying)
	{
		if (!IsValid(AudioComponent))
		{
			AudioComponent = UGameplayStatics::SpawnSound2D(this, TerrorAudio, 0.6f, 1.0f, 0, nullptr, false, false);

			AudioComponent->OnAudioFinished.AddDynamic(this, &UDCTerrorRadiusComponent::OnAudioFinished);
		}
		else
		{
			AudioComponent->Play(0.0f);
		}

		bIsPlaying = true;
	}
}
