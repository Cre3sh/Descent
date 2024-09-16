// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUICryogenicControlsWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDCUIOnCryogenicActivated);

class UButton;

UCLASS()
class UDCUICryogenicControlsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget override

	FDCUIOnCryogenicActivated OnCryogenicActivated;

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> ActivateButton = nullptr;

private:
	UFUNCTION()
	void OnActivated();
};
