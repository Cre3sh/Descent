// Fill out your copyright notice in the Description page of Project Settings.

#include "DCPlayerHUD.h"

#include <Components/Image.h>
#include <MediaPlayer.h>
#include <GameFramework/GameStateBase.h>
#include <GameFramework/PlayerState.h>

#include "Player/DCPlayerCharacter.h"
#include "Player/DCPlayerController.h"
#include "Player/DCPlayerState.h"

void UDCPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	check(MediaPlayer);

	// Why do we open this on ctor? Because we don't want the game to hitch when the player gets caught by the monster
	// as it tries to load the media source etc.

	MediaPlayer->OpenSource(DeathSceneMediaSource);
}

void UDCPlayerHUD::NativeDestruct()
{
	Super::NativeDestruct();

	ADCPlayerCharacter* const OwningPlayer = GetOwningPlayerPawn<ADCPlayerCharacter>();
	if (!IsValid(OwningPlayer))
	{
		return;
	}

	ADCPlayerState* const OwningPlayerState = OwningPlayer->GetPlayerState<ADCPlayerState>();

	check(OwningPlayerState);

	OwningPlayerState->OnPlayerDied.RemoveAll(this);
}

void UDCPlayerHUD::OnOwningPlayerStateSet(ADCPlayerState* PlayerState)
{
	check(PlayerState);

	PlayerState->OnPlayerDied.AddDynamic(this, &UDCPlayerHUD::OnPlayerDied);
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
	MediaPlayer->OnEndReached.AddDynamic(this, &UDCPlayerHUD::OnJumpscareFinished);

	PlayAnimation(PlayerDiedAnimation);
}

void UDCPlayerHUD::OnJumpscareFinished()
{
	DeathScreenVideoImage->SetVisibility(ESlateVisibility::Collapsed);

	for (const APlayerState* const PlayerState : GetWorld()->GetGameState()->PlayerArray)
	{
		ADCPlayerCharacter* const PlayerCharacter = PlayerState->GetPawn<ADCPlayerCharacter>();
		ADCPlayerCharacter* const OwningPlayer = GetOwningPlayerPawn<ADCPlayerCharacter>();
		if (OwningPlayer == PlayerCharacter)
		{
			continue;
		}

		GetOwningPlayer<ADCPlayerController>()->SpectatePlayer(PlayerCharacter);
	}

	MediaPlayer->OnEndReached.RemoveAll(this);
}