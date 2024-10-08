// Fill out your copyright notice in the Description page of Project Settings.

#include "DCUISceneManager.h"

#include <Components/CanvasPanel.h>
#include <Components/CanvasPanelSlot.h>

#include "DCUISceneWidget.h"
#include "Player/DCPlayerCharacter.h"
#include "UI/Scene/Data/DCSceneData.h"

FReply UDCUISceneManager::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FName PressedKey = InKeyEvent.GetKey().GetFName();

	if (PressedKey == "Escape")
	{
		OnPlayerPressedEscape();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UDCUISceneManager::NativeDestruct()
{
	Super::NativeDestruct();
}

UDCUISceneWidget* UDCUISceneManager::OpenScene(FGameplayTag InSceneTag)
{
	const UWorld* const World = GetWorld();

	check(Canvas);
	check(World);

	APlayerController* const PlayerController = GetOwningPlayer<APlayerController>();

	check(PlayerController);

	// Check if this scene has already been constructed
	if (ExistingScenes.Contains(InSceneTag))
	{
		UDCUISceneWidget* const ConstructedScene = ExistingScenes.FindRef(InSceneTag);
		if (IsValid(ConstructedScene))
		{
			if (IsValid(OpenedScene))
			{
				OpenedScene->CloseScene();
			}

			OpenedScene = ConstructedScene;
			OpenedSceneTag = InSceneTag;

			OpenedScene->OnSceneOpened();

			return OpenedScene;
		}

		if (OpenedSceneTag == InSceneTag)
		{
			return OpenedScene;
		}

		OpenedSceneTag = InSceneTag;
		OpenedScene->OnSceneOpened();

		return OpenedScene;
	}

	// We haven't created this widget before so create it now

	check(ScenesData);

	UDCUISceneWidget* const NewScene = CreateWidget<UDCUISceneWidget>(PlayerController, ScenesData->ScenesMap.FindRef(InSceneTag));
	if (IsValid(NewScene))
	{
		// Add the child widget to the CanvasPanel
		UCanvasPanelSlot* CanvasSlot = Canvas->AddChildToCanvas(NewScene);

		if (CanvasSlot)
		{
			// Set anchors to fill the whole screen
			CanvasSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));

			// Set offsets to zero
			CanvasSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
		}

		ExistingScenes.Add(InSceneTag, NewScene);

		OpenedSceneTag = InSceneTag;

		NewScene->SetSceneTag(OpenedSceneTag);
		NewScene->OnSceneOpened();

		if (IsValid(OpenedScene))
		{
			OpenedScene->CloseScene();
		}

		OpenedScene = NewScene;
	}

	return NewScene;
}

UUserWidget* UDCUISceneManager::GetSceneByTag(FGameplayTag InSceneTag) const
{
	return ExistingScenes.FindRef(InSceneTag);
}

void UDCUISceneManager::OnPlayerPressedEscape()
{
	UDCUISceneWidget* const SceneWidget = Cast<UDCUISceneWidget>(ExistingScenes.FindRef(OpenedSceneTag));
	
	if (IsValid(SceneWidget))
	{
		SceneWidget->CloseScene();
	
		OpenedSceneTag = FGameplayTag::EmptyTag;
	}
}