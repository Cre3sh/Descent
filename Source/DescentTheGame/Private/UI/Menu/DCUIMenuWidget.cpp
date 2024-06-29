// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/DCUIMenuWidget.h"

#include <Components/Button.h>
#include <Components/EditableText.h>

#include <AdvancedSteamFriendsLibrary.h>
#include <AdvancedSessionsLibrary.h>

#include "Base/DCAdvancedGameInstance.h"

void UDCUIMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(EditablePlayerNameText);

	FBPUniqueNetId UniqueNetID;
	UAdvancedSessionsLibrary::GetUniqueNetID(Cast<APlayerController>(GetOwningPlayer()), UniqueNetID);

	FText PlayerNameText = FText::FromString(UAdvancedSteamFriendsLibrary::GetSteamPersonaName(UniqueNetID));

	if (!PlayerNameText.IsEmpty())
	{
		EditablePlayerNameText->SetText(PlayerNameText);
	}

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

	check(EditablePlayerNameText);

	EditablePlayerNameText->OnTextChanged.AddDynamic(this, &UDCUIMenuWidget::OnPlayerNameChanged);

	Super::NativeDestruct();
}

void UDCUIMenuWidget::OnPlayerNameChanged(const FText& Text)
{
	UDCAdvancedGameInstance* const GameInstance = GetGameInstance<UDCAdvancedGameInstance>();

	check(GameInstance);

	GameInstance->LocalPlayerName = Text;
}
