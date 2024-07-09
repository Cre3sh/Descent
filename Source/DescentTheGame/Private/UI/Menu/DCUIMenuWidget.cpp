// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/DCUIMenuWidget.h"

#include <Components/Button.h>
#include <Components/EditableText.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/GameUserSettings.h>

#include <AdvancedSteamFriendsLibrary.h>
#include <AdvancedSessionsLibrary.h>

#include "DCUIMenuElementButton.h"
#include "Base/DCAdvancedGameInstance.h"

void UDCUIMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(HostButton);
	check(JoinButton);
	check(OptionsButton);
	check(QuitButton)
	check(EditablePlayerNameText);

	EditablePlayerNameText->OnTextChanged.AddDynamic(this, &UDCUIMenuWidget::OnPlayerNameChanged);

	Super::NativeDestruct();

	FBPUniqueNetId UniqueNetID;
	UAdvancedSessionsLibrary::GetUniqueNetID(Cast<APlayerController>(GetOwningPlayer()), UniqueNetID);

	FText PlayerNameText = FText::FromString(UAdvancedSteamFriendsLibrary::GetSteamPersonaName(UniqueNetID));

	if (!PlayerNameText.IsEmpty())
	{	
		EditablePlayerNameText->SetText(PlayerNameText);

		UDCAdvancedGameInstance* const GameInstance = GetGameInstance<UDCAdvancedGameInstance>();

		check(GameInstance);

		GameInstance->LocalPlayerName = PlayerNameText;
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

	HostButton->SetButtonText(FText::FromString("Host"));
	JoinButton->SetButtonText(FText::FromString("Join"));
	OptionsButton->SetButtonText(FText::FromString("Options"));
	QuitButton->SetButtonText(FText::FromString("Quit"));

	// Can assume here we have a valid button otherwise game would have crashed already
	QuitButton->GetButton()->OnPressed.AddDynamic(this, &UDCUIMenuWidget::OnQuitButtonPressed);

	// TODO remove this once the options menu is implemented
	UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
	UserSettings->SetOverallScalabilityLevel(2);

	UserSettings->SetReflectionQuality(2);
	UserSettings->SetShadingQuality(2);
	UserSettings->SetFoliageQuality(2);
	UserSettings->SetShadowQuality(2);
	UserSettings->SetTextureQuality(2);
	UserSettings->SetPostProcessingQuality(2);
	UserSettings->SetAntiAliasingQuality(2);
	UserSettings->SetGlobalIlluminationQuality(2);
	UserSettings->SetViewDistanceQuality(2);
	UserSettings->SetVisualEffectQuality(2);

	UserSettings->ApplySettings(false);
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
}

void UDCUIMenuWidget::OnPlayerNameChanged(const FText& Text)
{
	UDCAdvancedGameInstance* const GameInstance = GetGameInstance<UDCAdvancedGameInstance>();

	check(GameInstance);

	GameInstance->LocalPlayerName = Text;
}

void UDCUIMenuWidget::OnQuitButtonPressed()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, false);
}
