// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Systems/DCUISceneWidget.h"

#include "DCUIMenuWidget.generated.h"

class UMediaPlayer;
class UMediaSource;
class UWidgetSwitcher;
class UDCUIMenuElementButton;
class UEditableText;

UCLASS()
class UDCUIMenuWidget : public UDCUISceneWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget override

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> HostButton = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> JoinButton = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> OptionsButton = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> QuitButton = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UEditableText> EditablePlayerNameText = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> ScreenSwitcher = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> MediaSource = nullptr;

private:
	UFUNCTION()
	void OnPlayerNameChanged(const FText& Text);

	UFUNCTION()
	void OnQuitButtonPressed();

	UFUNCTION()
	void OnOptionsButtonPressed();

	UFUNCTION()
	void OnMediaOpened(FString OpenedUrl);
	
	UFUNCTION()
	void OnMediaFinished();

	FTimerHandle ExitAnimationFinishHandle;
};