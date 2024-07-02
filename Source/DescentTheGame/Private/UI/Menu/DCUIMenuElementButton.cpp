// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/DCUIMenuElementButton.h"

#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>

void UDCUIMenuElementButton::NativeConstruct()
{
	Super::NativeConstruct();

	check(Button);
	check(LeftBorderImage);
	check(ButtonText);

	Button->OnHovered.AddDynamic(this, &UDCUIMenuElementButton::OnButtonHovered);
	Button->OnPressed.AddDynamic(this, &UDCUIMenuElementButton::OnButtonPressed);
	Button->OnUnhovered.AddDynamic(this, &UDCUIMenuElementButton::OnButtonUnhovered);

	LeftBorderImage->SetColorAndOpacity(UnHoveredColor);
	Button->SetColorAndOpacity(UnHoveredTextColor);
}

void UDCUIMenuElementButton::NativeDestruct()
{
	Super::NativeDestruct();

	check(Button);

	Button->OnHovered.RemoveAll(this);
	Button->OnPressed.RemoveAll(this);
	Button->OnHovered.RemoveAll(this);
}

void UDCUIMenuElementButton::SetButtonText(const FText InText) const
{
	ButtonText->SetText(InText);
}

UButton* UDCUIMenuElementButton::GetButton() const
{
	return Button;
}

void UDCUIMenuElementButton::OnButtonHovered()
{
	LeftBorderImage->SetColorAndOpacity(HoveredColor);
	Button->SetColorAndOpacity(HoveredColor);
}

void UDCUIMenuElementButton::OnButtonPressed()
{
	LeftBorderImage->SetColorAndOpacity(PressedColor);
	Button->SetColorAndOpacity(PressedColor);
}

void UDCUIMenuElementButton::OnButtonUnhovered()
{
	LeftBorderImage->SetColorAndOpacity(UnHoveredColor);
	Button->SetColorAndOpacity(UnHoveredTextColor);
}