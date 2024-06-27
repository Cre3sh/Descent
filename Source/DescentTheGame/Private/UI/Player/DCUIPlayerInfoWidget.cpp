// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/DCUIPlayerInfoWidget.h"

#include <Components/TextBlock.h>

#include <AdvancedSteamFriendsLibrary.h>
#include <AdvancedSessionsLibrary.h>

void UDCUIPlayerInfoWidget::SetTrackedPlayer(APlayerState* PlayerState)
{
	TrackedPlayerState = PlayerState;

	check(PlayerNameText);

	APlayerController* const OwningPlayerController = GetOwningPlayer();
	if (!IsValid(OwningPlayerController))
	{
		return;
	}

	if (!OwningPlayerController->IsLocalPlayerController())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (!IsValid(PlayerState))
	{
		return;
	}

	const APawn* const Pawn = PlayerState->GetPawn();
	if (!IsValid(Pawn))
	{
		return;
	}

	FBPUniqueNetId UniqueNetID;
	UAdvancedSessionsLibrary::GetUniqueNetID(Cast<APlayerController>(Pawn->GetController()), UniqueNetID);

	const FText PlayerName = FText::FromString(UAdvancedSteamFriendsLibrary::GetSteamPersonaName(UniqueNetID));

	if (PlayerName.IsEmpty())
	{
		int32 ran = FMath::RandRange(2312, 565435);
		PlayerNameText->SetText(FText::AsNumber(ran));
		return;
	}

	PlayerNameText->SetText(PlayerName);

	GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, PlayerName.ToString());

	UE_LOG(LogTemp, Error, TEXT("PLAYER NAME: %s"), *PlayerName.ToString());
}
