// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/DCUIActionPromptWidget.h"

void UDCUIActionPromptWidget::SetActionText(FText ActionText) const
{
	ActionTextBlock->SetText(ActionText);
}
