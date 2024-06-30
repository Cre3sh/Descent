// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/DCUIPlayerInfoWidget.h"

#include <Components/TextBlock.h>
#include <GameplayTagContainer.h>
#include <Components/Image.h>

#include "DCUIPlayerInteractionInfoData.h"

void UDCUIPlayerInfoWidget::SetTrackedPlayer(const FText& PlayerName)
{
	check(PlayerNameText);

	PlayerNameText->SetText(PlayerName);
	UE_LOG(LogTemp, Warning, TEXT("Name %s"), *PlayerName.ToString());
}

void UDCUIPlayerInfoWidget::PlayerStartedInteraction(FGameplayTag InteractionTag)
{
	check(InteractionImage);
	check(PlayerInteractionInfoData);

	UMaterialInstance* const MaterialInstance = PlayerInteractionInfoData->InteractionMaterials.FindRef(InteractionTag);
	if (!IsValid(MaterialInstance))
	{
		return;
	}

	InteractionImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	InteractionImage->SetBrushFromMaterial(MaterialInstance);
}

void UDCUIPlayerInfoWidget::PlayerStoppedInteraction()
{
	InteractionImage->SetVisibility(ESlateVisibility::Collapsed);
}
