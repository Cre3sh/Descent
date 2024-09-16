// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Systems/DCUISceneWidget.h"

#include <Blueprint/WidgetBlueprintLibrary.h>

#include "DCUISceneManager.h"
#include "Player/DCPlayerCharacter.h"

void UDCUISceneWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
}

FReply UDCUISceneWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FName PressedKey = InKeyEvent.GetKey().GetFName();

	if (PressedKey == "Escape")
	{
		const ADCPlayerCharacter* const PlayerCharacter = GetOwningPlayerPawn<ADCPlayerCharacter>();
		if (IsValid(PlayerCharacter))
		{
			UDCUISceneManager* const SceneManager = PlayerCharacter->GetSceneManager();

			check(SceneManager);

			SceneManager->OnPlayerPressedEscape();
		}
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UDCUISceneWidget::SetSceneTag(FGameplayTag InSceneTag)
{
	SceneTag = InSceneTag;
}

FGameplayTag UDCUISceneWidget::GetSceneTag() const
{
	return SceneTag;
}

void UDCUISceneWidget::OnSceneOpened()
{
	bSceneClosed = false;

	const UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}

	APlayerController* const PlayerController = GetOwningPlayer();
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->bShowMouseCursor = true;
	PlayerController->bEnableClickEvents = true;
	PlayerController->bEnableMouseOverEvents = true;

	UGameViewportClient* const GameViewportClient = World->GetGameViewport();

	check(GameViewportClient);

	GameViewportClient->SetMouseLockMode(EMouseLockMode::LockInFullscreen);

	GameViewportClient->SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PlayerController, this, EMouseLockMode::LockInFullscreen);

	SetKeyboardFocus();

	SetFocus();

	if (OnSceneEnterAnimation)
	{
		PlayAnimation(OnSceneEnterAnimation);
	}

	SetVisibility(ESlateVisibility::Visible);
}

void UDCUISceneWidget::CloseScene(const bool bIgnoreInput)
{
	if (bSceneClosed)
	{
		return;
	}

	const UWorld* const World = GetWorld();
	if (!IsValid(World))
	{
		return;
	}
	
	APlayerController* const PlayerController = GetOwningPlayer<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	if (!bIgnoreInput)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;

		UGameViewportClient* const GameViewportClient = World->GetGameViewport();

		check(GameViewportClient);

		GameViewportClient->SetMouseLockMode(EMouseLockMode::LockAlways);

		GameViewportClient->SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently);

		UWidgetBlueprintLibrary::SetInputMode_GameOnly(PlayerController);
	}

	SetVisibility(ESlateVisibility::Collapsed);
	bSceneClosed = true;
}
