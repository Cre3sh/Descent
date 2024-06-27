// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Scene/Interactables/CommandTablet/DCUISecurityDataEntryWidget.h"

#include <Components/DynamicEntryBox.h>

#include "DCUISecurityMinigameArrowWidget.h"

void UDCUISecurityDataEntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(DynamicEntryBox);

	const uint8 EntryNum = FMath::RandRange(4, 12);

	for (int i = 0; i < EntryNum; i++)
	{
		DynamicEntryBox->CreateEntry();
	}

	PlayAnimation(OnEntriesSetAnimation);
}

FReply UDCUISecurityDataEntryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (bIsResettingPuzzle)
	{
		return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	}

	check(DynamicEntryBox);

	const FName PressedKey = InKeyEvent.GetKey().GetFName();

	EDCUIArrowDirection PressedDirection;

	if (PressedKey == "W")
	{
		PressedDirection = EDCUIArrowDirection::Forward;
	}
	else if (PressedKey == "D")
	{
		PressedDirection = EDCUIArrowDirection::Right;
	}
	else if (PressedKey == "S")
	{
		PressedDirection = EDCUIArrowDirection::Down;
	}
	else if (PressedKey == "A")
	{
		PressedDirection = EDCUIArrowDirection::Left;
	}
	else
	{
		return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	}

	TArray<UUserWidget*> ArrowEntries = DynamicEntryBox->GetAllEntries();

	if (ArrowEntries.IsValidIndex(CurrentEntryIndex))
	{
		UDCUISecurityMinigameArrowWidget* const EntryWidget = Cast<UDCUISecurityMinigameArrowWidget>(ArrowEntries[CurrentEntryIndex]);

		check(EntryWidget);

		if (EntryWidget->OnPlayerPressedDirection(PressedDirection))
		{
			// We win
			if (CurrentEntryIndex)
			{
				
			}
			CurrentEntryIndex++;
		}
		else
		{
			for (UUserWidget* const Entry : ArrowEntries)
			{
				UDCUISecurityMinigameArrowWidget* const ArrowWidget = Cast<UDCUISecurityMinigameArrowWidget>(Entry);

				check(ArrowWidget);

				ArrowWidget->OnPlayerPressedWrongDirection();
			}

			bIsResettingPuzzle = true;

			const UWorld* const World = GetWorld();

			check(World);

			World->GetTimerManager().SetTimer(ResetPuzzleTimerHandle, this, &UDCUISecurityDataEntryWidget::ResetPuzzle, 1.0f);
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UDCUISecurityDataEntryWidget::ResetPuzzle()
{
	check(DynamicEntryBox);

	TArray<UUserWidget*> ArrowEntries = DynamicEntryBox->GetAllEntries();

	for (UUserWidget* const Entry : ArrowEntries)
	{
		UDCUISecurityMinigameArrowWidget* const ArrowWidget = Cast<UDCUISecurityMinigameArrowWidget>(Entry);

		check(ArrowWidget);

		ArrowWidget->ResetState();
	}

	bIsResettingPuzzle = false;
	CurrentEntryIndex = 0;
}
