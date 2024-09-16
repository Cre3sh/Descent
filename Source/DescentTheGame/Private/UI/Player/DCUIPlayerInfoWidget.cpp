// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Player/DCUIPlayerInfoWidget.h"

#include <Components/TextBlock.h>
#include <GameplayTagContainer.h>
#include <Components/Image.h>
#include <Kismet/GameplayStatics.h>

#include "DCUIPlayerInteractionInfoData.h"
#include "Player/DCPlayerCharacter.h"

void UDCUIPlayerInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PlayerController))
	{
		return;
	}

	ADCPlayerCharacter* const PlayerCharacter = PlayerController->GetPawn<ADCPlayerCharacter>();
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	ADCPlayerCharacter* const OwningPlayer = WeakTrackedPlayer.Get();
	if (!IsValid(OwningPlayer))
	{
		return;
	}

	if(FVector::Distance(PlayerCharacter->GetActorLocation(), OwningPlayer->GetActorLocation()) >= 2500.0f)
	{
		PlayerNameText->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		PlayerNameText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UDCUIPlayerInfoWidget::SetTrackedPlayer(ADCPlayerCharacter* const PlayerCharacter, const FText& PlayerName)
{
	check(PlayerNameText);

	WeakTrackedPlayer = PlayerCharacter;
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
