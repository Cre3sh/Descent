// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/DCPlayerController.h"

#include <Camera/CameraActor.h>

#include "DCPlayerCharacter.h"

void ADCPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	GetWorld()->GetTimerManager().ClearTimer(DestroyPlayerHandle);
}

void ADCPlayerController::SpectatePlayer(ADCPlayerCharacter* PlayerCharacter)
{
	DestroyOwningPawn();

	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(DestroyPlayerHandle, FTimerDelegate::CreateWeakLambda(this, [this, PlayerCharacter]()
	{
		SetViewTargetWithBlend(PlayerCharacter->GetSpectatorCamera());
	}), 2.50f, false);
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