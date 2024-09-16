// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Scene/Interactables/CommandTablet/DCUISecurityMinigameArrowWidget.h"

#include <Components/Overlay.h>

void UDCUISecurityMinigameArrowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResetState();
}

void UDCUISecurityMinigameArrowWidget::ResetState()
{
	uint8 RandomNumber = FMath::RandRange(0, 3);

	CurrentDirection = static_cast<EDCUIArrowDirection>(RandomNumber);

	check(ImageOverlay);

	switch (CurrentDirection)
	{
		case EDCUIArrowDirection::Forward:
		{
			ImageOverlay->SetRenderTransformAngle(0.0f);
		}
		break;

		case EDCUIArrowDirection::Right:
		{
			ImageOverlay->SetRenderTransformAngle(90.0f);
		}
		break;

		case EDCUIArrowDirection::Down:
		{
			ImageOverlay->SetRenderTransformAngle(180.0f);	
		}
		break;

		case EDCUIArrowDirection::Left:
		{
			ImageOverlay->SetRenderTransformAngle(270.0f);	
		}
		break;
	}

	PlayAnimation(OnStateSetAnimation);
}

bool UDCUISecurityMinigameArrowWidget::OnPlayerPressedDirection(EDCUIArrowDirection Direction)
{
	if (Direction == CurrentDirection)
	{
		PlayAnimation(OnSuccessfulPressAnimation);

		return true;
	}

	return false;
}

void UDCUISecurityMinigameArrowWidget::OnPlayerPressedWrongDirection()
{
	PlayAnimation(OnIncorrectPressAnimation);
}
