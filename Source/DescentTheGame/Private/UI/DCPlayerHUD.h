// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCPlayerHUD.generated.h"

class UDCUIActionPromptWidget;
class ADCPlayerState;
class ADCPlayerCharacter;
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

	void DisplayActionPrompt(FText ActionText) const;
	void HideActionPrompt() const;

	void OnOwningPlayerStateSet(ADCPlayerState* PlayerState);

	void HandleHaltEvent();

protected:
	UFUNCTION()
	void OnObjectiveSet(FString NewObjectiveString);
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> DeathScreenVideoImage = nullptr;
	
	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> PlayerDiedAnimation = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ObjectiveSetAnimation = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> HaltEventAnimation = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> ObjectiveCompleteAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UDCUIActionPromptWidget> ActionPromptWidget = nullptr;

	// Media Player used to play cutscenes
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> DeathSceneMediaSource = nullptr;

	UPROPERTY(BlueprintReadOnly)
	FString CurrentObjectiveText = "";

private:
	UFUNCTION()
	void OnPlayerDied();

	UFUNCTION()
	void OnJumpscareFinished();

	UFUNCTION()
	void OnObjectiveCompleted(FString String);
};
