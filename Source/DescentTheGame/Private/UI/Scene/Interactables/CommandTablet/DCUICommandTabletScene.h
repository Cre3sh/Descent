// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/Image.h>

#include "UI/Systems/DCUISceneWidget.h"
#include "DCUICommandTabletScene.generated.h"

class ADCCryogenicChamber;
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
	virtual void CloseScene(const bool bIgnoreInput) override;
	virtual void OnSceneOpened() override;
	// End UDCUISceneWidget override

	void SetTablet(ADCCommandTablet* CommandTablet);

	void SetUseCryogenics(const bool bUseCryogenics, ADCCryogenicChamber* const CryogenicChamber = nullptr);

protected:
	UFUNCTION()
	void OnEnvironmentButtonPressed();

	UFUNCTION()
	void OnCryogenicsButtonPressed();

	UFUNCTION()
	void OnSecurityButtonPressed();

	UFUNCTION()
	void OnVideoOpened(FString OpenedUrl);

	UFUNCTION()
	void OnVideoFinished();

	UFUNCTION()
	void OnCryogenicsActivated();

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
	TObjectPtr<UButton> CryogenicsButton = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> SecurityButton = nullptr;

private:
	TWeakObjectPtr<ADCCryogenicChamber> CryogenicChamberWeak = nullptr;
	TWeakObjectPtr<ADCCommandTablet> WeakTablet = nullptr;
};
