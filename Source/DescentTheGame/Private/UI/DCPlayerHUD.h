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
	virtual void NativeDestruct() override;
	// End UUserWidget override

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> DeathScreenVideoImage = nullptr;
	
	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PlayerDiedAnimation = nullptr;

	// Media Player used to play cutscenes
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> DeathSceneMediaSource = nullptr;

private:
	UFUNCTION()
	void OnPlayerDied();

	UFUNCTION()
	void OnJumpscareFinished();
};
