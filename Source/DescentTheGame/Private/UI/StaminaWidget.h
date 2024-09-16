// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "StaminaWidget.generated.h"


/**
 *
 */
UCLASS()
class DESCENTTHEGAME_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()
private:

public:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* StaminaBar;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeOut;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* FadeIn;



protected:


	void NativeConstruct() override;



};