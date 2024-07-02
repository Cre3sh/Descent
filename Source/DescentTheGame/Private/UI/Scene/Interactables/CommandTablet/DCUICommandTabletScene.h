// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/Image.h>

#include "UI/Systems/DCUISceneWidget.h"
#include "DCUICommandTabletScene.generated.h"

class ADCCommandTablet;
class UMediaSource;
class UMediaPlayer;
class UWidgetSwitcher;
class UButton;

UCLASS()
class UDCUICommandTabletScene : public UDCUISceneWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget override

	// Begin UDCUISceneWidget override
	virtual void CloseScene() override;
	virtual void OnSceneOpened() override;
	// End UDCUISceneWidget override

	void SetTablet(ADCCommandTablet* CommandTablet);

protected:
	UFUNCTION()
	void OnEnvironmentButtonPressed();

	UFUNCTION()
	void OnSecurityButtonPressed();

	UFUNCTION()
	void OnVideoOpened(FString OpenedUrl);

	UFUNCTION()
	void OnVideoFinished();

	UFUNCTION()
	void OnPuzzleComplete();

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> BootUpVideoImage = nullptr;

	// Media Player used to play the boot up video
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> BootUpMediaSource = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> ScreenSwitcher = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> EnvironmentButton = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> SecurityButton = nullptr;

private:
	TWeakObjectPtr<ADCCommandTablet> WeakTablet = nullptr;
};
