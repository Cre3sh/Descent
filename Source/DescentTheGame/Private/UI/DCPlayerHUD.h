// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCPlayerHUD.generated.h"

class UDCSceneData;
class UMediaSource;
class UMediaPlayer;
class UImage;

UCLASS()
class UDCPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	// End UUserWidget override

	void OnPlayerDied();

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> DeathScreenVideoImage = nullptr;

	// Media Player used to play cutscenes
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> DeathSceneMediaSource = nullptr;
};
