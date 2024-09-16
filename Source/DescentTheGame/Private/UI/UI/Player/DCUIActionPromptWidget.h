// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>
#include <Components/TextBlock.h>

#include "DCUIActionPromptWidget.generated.h"

UCLASS()
class UDCUIActionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetActionText(FText ActionText) const;

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> ActionTextBlock = nullptr;
};