// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Scene/Interactables/CommandTablet//DCUICommandTabletScene.h"

#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <MediaPlayer.h>

#include "DCUICryogenicControlsWidget.h"
#include "DCUISecurityDataEntryWidget.h"
#include "Environment/DCCryogenicChamber.h"
#include "Interactable/DCCommandTablet.h"
#include "Player/DCPlayerCharacter.h"

void UDCUICommandTabletScene::NativeConstruct()
{
	Super::NativeConstruct();

	check(EnvironmentButton);
	check(SecurityButton);
	check(ScreenSwitcher);

	EnvironmentButton->OnClicked.AddDynamic(this, &UDCUICommandTabletScene::OnEnvironmentButtonPressed);
	CryogenicsButton->OnClicked.AddDynamic(this, &UDCUICommandTabletScene::OnCryogenicsButtonPressed);
	SecurityButton->OnClicked.AddDynamic(this, &UDCUICommandTabletScene::OnSecurityButtonPressed);

	UDCUISecurityDataEntryWidget* const SecurityDataEntryWidget = Cast<UDCUISecurityDataEntryWidget>(ScreenSwitcher->GetWidgetAtIndex(1));

	check(SecurityDataEntryWidget);

	SecurityDataEntryWidget->OnPuzzleCompleted.AddDynamic(this, &UDCUICommandTabletScene::OnPuzzleComplete);

	UDCUICryogenicControlsWidget* const CryogenicControlsWidget = Cast<UDCUICryogenicControlsWidget>(ScreenSwitcher->GetWidgetAtIndex(2));

	check(CryogenicControlsWidget);

	CryogenicControlsWidget->OnCryogenicActivated.AddDynamic(this, &UDCUICommandTabletScene::OnCryogenicsActivated);
}

void UDCUICommandTabletScene::NativeDestruct()
{
	Super::NativeDestruct();

	UDCUISecurityDataEntryWidget* const SecurityDataEntryWidget = Cast<UDCUISecurityDataEntryWidget>(ScreenSwitcher->GetWidgetAtIndex(1));

	check(SecurityDataEntryWidget);

	SecurityDataEntryWidget->OnPuzzleCompleted.RemoveAll(this);
}

void UDCUICommandTabletScene::CloseScene()
{
	Super::CloseScene();

	const ADCPlayerCharacter* const PlayerCharacter = GetOwningPlayerPawn<ADCPlayerCharacter>();
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	ADCCommandTablet* const CommandTablet = Cast<ADCCommandTablet>(PlayerCharacter->GetLastInteractedObject());
	if (!IsValid(CommandTablet))
	{
		return;
	}

	CommandTablet->Server_SetIsBeingInteracted(false);

	check(MediaPlayer);

	MediaPlayer->OnMediaOpened.RemoveAll(this);
	MediaPlayer->OnEndReached.RemoveAll(this);
}

void UDCUICommandTabletScene::OnSceneOpened()
{
	Super::OnSceneOpened();

	ScreenSwitcher->SetVisibility(ESlateVisibility::Collapsed);
	BootUpVideoImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	check(MediaPlayer);

	MediaPlayer->OpenSource(BootUpMediaSource);

	if (!MediaPlayer->IsReady())
	{
		MediaPlayer->OnMediaOpened.AddDynamic(this, &UDCUICommandTabletScene::OnVideoOpened);
	}
	else
	{
		MediaPlayer->Rewind();
		MediaPlayer->Play();
	}

	MediaPlayer->OnEndReached.AddDynamic(this, &UDCUICommandTabletScene::OnVideoFinished);
}

void UDCUICommandTabletScene::SetTablet(ADCCommandTablet* CommandTablet)
{
	WeakTablet = CommandTablet;
}

void UDCUICommandTabletScene::SetUseCryogenics(const bool bUseCryogenics, ADCCryogenicChamber* const CryogenicChamber)
{
	if (!bUseCryogenics)
	{
		return;
	}

	CryogenicChamberWeak = CryogenicChamber;
	CryogenicsButton->SetVisibility(ESlateVisibility::Visible);
}

void UDCUICommandTabletScene::OnEnvironmentButtonPressed()
{
	check(ScreenSwitcher);

	ScreenSwitcher->SetActiveWidgetIndex(0);
}

void UDCUICommandTabletScene::OnCryogenicsButtonPressed()
{
	check(ScreenSwitcher);

	ScreenSwitcher->SetActiveWidgetIndex(2);
}

void UDCUICommandTabletScene::OnSecurityButtonPressed()
{
	check(ScreenSwitcher);

	ScreenSwitcher->SetActiveWidgetIndex(1);

	UWidget* const ActiveWidget = ScreenSwitcher->GetActiveWidget();

	check(ActiveWidget);

	ActiveWidget->SetFocus();
	ActiveWidget->SetKeyboardFocus();
}

void UDCUICommandTabletScene::OnVideoOpened(FString OpenedUrl)
{
	check(MediaPlayer);

	MediaPlayer->Rewind();
	MediaPlayer->Play();
}

void UDCUICommandTabletScene::OnVideoFinished()
{
	check(BootUpVideoImage);
	check(ScreenSwitcher);

	BootUpVideoImage->SetVisibility(ESlateVisibility::Collapsed);
	ScreenSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// Why is anything with input just so... ugh
	SetFocus();
	SetKeyboardFocus();
}

void UDCUICommandTabletScene::OnCryogenicsActivated()
{
	ADCCommandTablet* const CommandTablet = WeakTablet.Get();
	if (!IsValid(CommandTablet))
	{
		return;
	}

	CommandTablet->OnCryogenicsActivated();
}

void UDCUICommandTabletScene::OnPuzzleComplete()
{
	ADCCommandTablet* const CommandTablet = WeakTablet.Get();
	if (!IsValid(CommandTablet))
	{
		return;
	}

	CommandTablet->OnPuzzleComplete();
}
