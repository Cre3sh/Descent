// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUIMenuWidget.generated.h"

class UDCUIMenuElementButton;
class UEditableText;

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> HostButton = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> JoinButton = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UDCUIMenuElementButton> QuitButton = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UEditableText> EditablePlayerNameText = nullptr;

private:
	UFUNCTION()
	void OnPlayerNameChanged(const FText& Text);

	UFUNCTION()
	void OnQuitButtonPressed();
};
