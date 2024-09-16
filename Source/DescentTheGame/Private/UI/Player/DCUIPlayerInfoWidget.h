// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>

#include "DCUIPlayerInfoWidget.generated.h"

class ADCPlayerCharacter;
class UImage;
class UDCUIPlayerInteractionInfoData;
struct FGameplayTag;
class UTextBlock;

UCLASS()
class UDCUIPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// End UUserWidget override

	// Sets a player to track on this widget
	void SetTrackedPlayer(ADCPlayerCharacter* const PlayerCharacter, const FText& PlayerName);

	void PlayerStartedInteraction(FGameplayTag InteractionTag);

	void PlayerStoppedInteraction();

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UImage> InteractionImage = nullptr;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TObjectPtr<UDCUIPlayerInteractionInfoData> PlayerInteractionInfoData = nullptr;

	TWeakObjectPtr<ADCPlayerCharacter> WeakTrackedPlayer = nullptr;
};
