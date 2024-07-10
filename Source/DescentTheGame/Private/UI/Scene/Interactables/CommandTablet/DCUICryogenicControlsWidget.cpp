// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Scene/Interactables/CommandTablet/DCUICryogenicControlsWidget.h"

#include <Components/Button.h>

void UDCUICryogenicControlsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	check(ActivateButton);

	ActivateButton->OnClicked.AddDynamic(this, &UDCUICryogenicControlsWidget::OnActivated);
}

void UDCUICryogenicControlsWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	ActivateButton->OnClicked.RemoveAll(this);
}

void UDCUICryogenicControlsWidget::OnActivated()
{
	if (OnCryogenicActivated.IsBound())
	{
		OnCryogenicActivated.Broadcast();
	}
}
