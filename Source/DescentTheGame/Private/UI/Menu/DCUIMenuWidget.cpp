// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/DCUIMenuWidget.h"

#include <Components/Button.h>

void UDCUIMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const UWorld* const World = GetWorld();
	if (IsValid(World))
	{
		APlayerController* const PlayerController = World->GetFirstPlayerController();
		if (IsValid(PlayerController))
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

void UDCUIMenuWidget::NativeDestruct()
{
	// temporary for testing purposes
	RemoveFromParent();
	UWorld* World = GetWorld();
	if (IsValid(World))
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (IsValid(PlayerController))
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(false);
		}
	}

	Super::NativeDestruct();
}