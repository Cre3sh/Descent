// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPlayerController.h"

#include <Net/VoiceConfig.h>
#include <Camera/CameraActor.h>

#include "DCPlayerCharacter.h"
#include "Base/DCGameMode.h"

void ADCPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	ADCGameMode* const GameMode = World->GetAuthGameMode<ADCGameMode>();
	if (!IsValid(GameMode))
	{
		return;
	}

	VoipTalkerComponent = Cast<UVOIPTalker>(AddComponentByClass(UVOIPTalker::StaticClass(), false, FTransform::Identity, false));

	check(VoipTalkerComponent);

	APlayerState* const OwningPlayerState = PlayerState;
	if (IsValid(OwningPlayerState))
	{
		VoipTalkerComponent->RegisterWithPlayerState(OwningPlayerState);

		UVOIPStatics::SetMicThreshold(-1.0f);

		USoundAttenuation* const SoundAttenuation = NewObject<USoundAttenuation>();
		if (!IsValid(SoundAttenuation))
		{
			return;
		}

		SoundAttenuation->Attenuation.bSpatialize = true;
	
		VoipTalkerComponent->Settings.AttenuationSettings = SoundAttenuation;
		VoipTalkerComponent->Settings.ComponentToAttachTo = InPawn->GetRootComponent();
	}
}

void ADCPlayerController::SpectatePlayer(ADCPlayerCharacter* PlayerCharacter)
{
	SetViewTarget(PlayerCharacter->GetSpectatorCamera());

	DestroyOwningPawn();
}

void ADCPlayerController::Server_DestroyOwningPawn_Implementation()
{
	DestroyOwningPawn();
}

void ADCPlayerController::DestroyOwningPawn()
{
	if (!HasAuthority())
	{
		Server_DestroyOwningPawn();
		return;
	}

	APawn* const OwningPawn = GetPawn();
	if (IsValid(OwningPawn))
	{
		OwningPawn->Destroy();
	}
}
