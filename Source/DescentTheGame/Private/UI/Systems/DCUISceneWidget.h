// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Blueprint/UserWidget.h>
#include <GameplayTagContainer.h>

#include "DCUISceneWidget.generated.h"

UCLASS()
class UDCUISceneWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual void NativeConstruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// End UUserWidget override

	void SetSceneTag(FGameplayTag InSceneTag);

	FGameplayTag GetSceneTag() const;

	virtual void OnSceneOpened();

	virtual void CloseScene(const bool bIgnoreInput = false);

protected:
	UPROPERTY(Transient, EditDefaultsOnly, meta=(BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> OnSceneEnterAnimation = nullptr;

	UPROPERTY(Transient, EditDefaultsOnly, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> OnSceneExitAnimation = nullptr;

	FGameplayTag SceneTag;

	bool bSceneClosed = false;
};
