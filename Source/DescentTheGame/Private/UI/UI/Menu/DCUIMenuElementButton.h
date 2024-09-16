// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUIMenuElementButton.generated.h"

class UTextBlock;
class UImage;
class UButton;

UCLASS()
class UDCUIMenuElementButton : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// End UUserWidget override

	void SetButtonText(const FText InText) const;

	UFUNCTION(BlueprintCallable)
	UButton* GetButton() const;

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> LeftBorderImage = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UButton> Button = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> ButtonText = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor HoveredColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor PressedColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor UnHoveredColor = FColor::Red;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor UnHoveredTextColor = FColor::Red;

private:
	UFUNCTION()
	void OnButtonHovered();
	
	UFUNCTION()
	void OnButtonPressed();

	UFUNCTION()
	void OnButtonUnhovered();
};	