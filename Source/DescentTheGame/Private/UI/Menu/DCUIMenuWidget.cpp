// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/DCUIMenuWidget.h"

#include <Components/Button.h>
#include <Components/EditableText.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/GameUserSettings.h>
#include <MediaPlayer.h>

#include <AdvancedSteamFriendsLibrary.h>
#include <AdvancedSessionsLibrary.h>

#include "DCUIMenuElementButton.h"
#include "Base/DCAdvancedGameInstance.h"
#include "DCMenuCharacter.h"
#include "UI/Systems/DCUISceneManager.h"
#include "UI/Tags/DCSceneTags.h"

void UDCUIMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(MediaPlayer);
	check(HostButton);
	check(JoinButton);
	check(OptionsButton);
	check(QuitButton)
	check(EditablePlayerNameText);

	MediaPlayer->OpenSource(MediaSource);
	MediaPlayer->OnMediaOpened.AddDynamic(this, &UDCUIMenuWidget::OnMediaOpened);
	MediaPlayer->OnEndReached.AddDynamic(this, &UDCUIMenuWidget::OnMediaFinished);

	EditablePlayerNameText->OnTextChanged.AddDynamic(this, &UDCUIMenuWidget::OnPlayerNameChanged);

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
	OptionsButton->GetButton()->OnPressed.AddDynamic(this, &UDCUIMenuWidget::OnOptionsButtonPressed);

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

void UDCUIMenuWidget::OnOptionsButtonPressed()
{
	ADCMenuCharacter* const MenuCharacter = GetOwningPlayerPawn<ADCMenuCharacter>();

	check(MenuCharacter);

	PlayAnimation(OnSceneExitAnimation);
	GetWorld()->GetTimerManager().SetTimer(ExitAnimationFinishHandle, FTimerDelegate::CreateWeakLambda(this, [this, MenuCharacter]()
	{
		CloseScene(true);
		MenuCharacter->SceneManager->OpenScene(Tag_UI_Scene_Options);
	}), 0.5f, false);
}

void UDCUIMenuWidget::OnMediaOpened(FString OpenedUrl)
{
	MediaPlayer->Play();

	MediaPlayer->OnMediaOpened.RemoveAll(this);
}

void UDCUIMenuWidget::OnMediaFinished()
{
	MediaPlayer->Rewind();
	MediaPlayer->Play();
}
