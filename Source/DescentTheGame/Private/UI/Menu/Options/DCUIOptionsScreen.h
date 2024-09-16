// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Systems/DCUISceneWidget.h"

#include "DCUIOptionsScreen.generated.h"

class UButton;
class UMediaPlayer;
class UMediaSource;

UCLASS()
class UDCUIOptionsScreen : public UDCUISceneWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	// End UUserWidget override
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaPlayer> MediaPlayer = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UMediaSource> MediaSource = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> BackButton = nullptr;

private:
	UFUNCTION()
	void OnMediaOpened(FString OpenedUrl);

	UFUNCTION()
	void OnMediaFinished();

	UFUNCTION()
	void OnPressedBack();

	FTimerHandle ExitAnimationFinishHandle;
};