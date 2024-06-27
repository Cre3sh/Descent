// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUISceneWidget.generated.h"

UCLASS()
class UDCUISceneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// End UUserWidget override

	virtual void OnSceneOpened();

	virtual void CloseScene();
};
