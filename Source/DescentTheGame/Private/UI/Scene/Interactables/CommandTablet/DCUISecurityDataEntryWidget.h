// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUISecurityDataEntryWidget.generated.h"

class UDynamicEntryBox;

UCLASS()
class UDCUISecurityDataEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// End UUserWidget override

protected:
	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> OnEntriesSetAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UDynamicEntryBox> DynamicEntryBox = nullptr;

private:
	void ResetPuzzle();

	uint8 CurrentEntryIndex = 0;

	bool bIsResettingPuzzle = false;

	FTimerHandle ResetPuzzleTimerHandle;
};
