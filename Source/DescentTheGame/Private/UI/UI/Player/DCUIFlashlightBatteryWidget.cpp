// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/DCUIFlashlightBatteryWidget.h"

#include <Components/Image.h>

#include "Player/DCPlayerCharacter.h"
#include "Player/Utility/DCFlashlightComponent.h"

void UDCUIFlashlightBatteryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const ADCPlayerCharacter* const PlayerCharacter = GetOwningPlayerPawn<ADCPlayerCharacter>();
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	if (PlayerCharacter->IsDead())
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else
	{
		SetVisibility(ESlateVisibility::HitTestInvisible);
	}

	UDCFlashlightComponent* const FlashlightComponent = PlayerCharacter->GetFlashlightComponent();

	if (!FlashlightComponent->IsOn())
	{
		FlashlightImage->SetColorAndOpacity(FLinearColor::Black);
		BatteryBar->SetRenderOpacity(0.0f);
	}
	else
	{
		FlashlightImage->SetColorAndOpacity(FLinearColor::White);
		BatteryBar->SetRenderOpacity(1);
	}

	check(FlashlightComponent);

	float FlashlightPercentage = FlashlightComponent->GetBatteryPercentage();

	BatteryBar->SetPercent(FlashlightPercentage / 100.0f);
}