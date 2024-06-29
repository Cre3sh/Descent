// Fill out your copyright notice in the Description page of Project Settings.

#include "DCPlayerHUD.h"

#include <Components/Image.h>
#include <MediaPlayer.h>

#include "Player/DCPlayerCharacter.h"

void UDCPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	check(MediaPlayer);

	// Why do we open this on ctor? Because we don't want the game to hitch when the player gets caught by the monster
	// as it tries to load the media source etc.

	MediaPlayer->OpenSource(DeathSceneMediaSource);
}

void UDCPlayerHUD::OnPlayerDied()
{
	check(DeathScreenVideoImage);
	check(MediaPlayer);

	DeathScreenVideoImage->SetVisibility(ESlateVisibility::Visible);

	const ADCPlayerCharacter* const PlayerCharacter = GetOwningPlayerPawn<ADCPlayerCharacter>();

	check(PlayerCharacter);

	UMediaSoundComponent* const MediaSoundComponent = PlayerCharacter->GetMediaSoundComponent();

	check(MediaSoundComponent);

	if (!MediaPlayer->IsReady())
	{
		UE_LOG(LogTemp, Error, TEXT("Media player wasn't ready for death scene!"));
		return;
	}
	
	MediaPlayer->Play();
}