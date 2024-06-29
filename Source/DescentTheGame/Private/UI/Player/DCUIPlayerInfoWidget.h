// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUIPlayerInfoWidget.generated.h"

class UTextBlock;

UCLASS()
class UDCUIPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Sets a player to track on this widget
	void SetTrackedPlayer(const FText& PlayerName);

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText = nullptr;
};
