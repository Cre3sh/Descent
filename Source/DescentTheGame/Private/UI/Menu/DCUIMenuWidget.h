// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUIMenuWidget.generated.h"

class UEditableText;
class UButton;

UCLASS()
class UDCUIMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;

	virtual void NativeDestruct() override;
	// End UUserWidget override

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> HostButton = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> JoinButton = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UEditableText> EditablePlayerNameText = nullptr;

private:
	UFUNCTION()
	void OnPlayerNameChanged(const FText& Text);
};
