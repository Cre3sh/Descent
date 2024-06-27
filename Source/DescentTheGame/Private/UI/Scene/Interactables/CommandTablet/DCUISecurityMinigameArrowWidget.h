// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUISecurityMinigameArrowWidget.generated.h"

class UOverlay;
class UImage;

UENUM()
enum class EDCUIArrowDirection : uint8
{
	Forward = 0,
	Right = 1,
	Down = 2,
	Left = 3
};

UCLASS()
class UDCUISecurityMinigameArrowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	// End UUserWidget override

	void ResetState();

	bool OnPlayerPressedDirection(EDCUIArrowDirection Direction);

	void OnPlayerPressedWrongDirection();

protected:
	// The image used for the arrow
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UOverlay> ImageOverlay = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnStateSetAnimation = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnSuccessfulPressAnimation = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnIncorrectPressAnimation = nullptr;

	EDCUIArrowDirection CurrentDirection;
};
