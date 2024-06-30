// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/DCUIPlayerInfoWidget.h"

#include <Components/TextBlock.h>

void UDCUIPlayerInfoWidget::SetTrackedPlayer(const FText& PlayerName)
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	check(PlayerNameText);

	APlayerController* const OwningPlayerController = GetOwningPlayer();
	if (!IsValid(OwningPlayerController))
	{
		return;
	}

	if (!OwningPlayerController->IsLocalPlayerController())
	{
		SetVisibility(ESlateVisibility::Collapsed);
	}

	PlayerNameText->SetText(PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("Name %s"), *PlayerName.ToString());
}
