// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/Options/DCUIOptionsScreen.h"

#include <MediaPlayer.h>
#include <Components/Button.h>

#include "UI/Menu/DCMenuCharacter.h"
#include "UI/Systems/DCUISceneManager.h"
#include "UI/Tags/DCSceneTags.h"

void UDCUIOptionsScreen::NativeConstruct()
{
	check(MediaPlayer);

	MediaPlayer->OpenSource(MediaSource);
	MediaPlayer->OnMediaOpened.AddDynamic(this, &UDCUIOptionsScreen::OnMediaOpened);
	MediaPlayer->OnEndReached.AddDynamic(this, &UDCUIOptionsScreen::OnMediaFinished);

	BackButton->OnClicked.AddDynamic(this, &UDCUIOptionsScreen::OnPressedBack);
}

void UDCUIOptionsScreen::OnMediaOpened(FString OpenedUrl)
{
	MediaPlayer->Play();

	MediaPlayer->OnMediaOpened.RemoveAll(this);
}

void UDCUIOptionsScreen::OnMediaFinished()
{
	MediaPlayer->Rewind();
	MediaPlayer->Play();
}

void UDCUIOptionsScreen::OnPressedBack()
{
	ADCMenuCharacter* const MenuCharacter = GetOwningPlayerPawn<ADCMenuCharacter>();

	PlayAnimation(OnSceneExitAnimation);
	GetWorld()->GetTimerManager().SetTimer(ExitAnimationFinishHandle, FTimerDelegate::CreateWeakLambda(this, [this, MenuCharacter]()
	{
		CloseScene(true);
		MenuCharacter->SceneManager->OpenScene(Tag_UI_Scene_MainMenu);
	}), 0.5f, false);
}
