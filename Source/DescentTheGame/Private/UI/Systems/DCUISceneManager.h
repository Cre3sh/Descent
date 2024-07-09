// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameplayTagContainer.h>
#include <CommonActivatableWidget.h>

#include "UI/Scene/Data/DCSceneData.h"

#include "DCUISceneManager.generated.h"

class UDCUISceneWidget;
class UCanvasPanel;
struct FGameplayTag;
class ADCPlayerCharacter;

UCLASS()
class UDCUISceneManager : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	// Begin UUserWidget override
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// End UUserWidget override

	// Opens a scene for the specified player character
	UDCUISceneWidget* OpenScene(ADCPlayerCharacter* PlayerCharacter, FGameplayTag InSceneTag);

	// Returns an existing scene from the specified gameplay tag
	UUserWidget* GetSceneByTag(FGameplayTag InSceneTag) const;

	void OnPlayerPressedEscape();

private:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> Canvas = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDCSceneData> ScenesData = nullptr;

	UPROPERTY(Transient)
	TMap<FGameplayTag, TObjectPtr<UDCUISceneWidget>> ExistingScenes = {};

	FGameplayTag OpenedSceneTag = FGameplayTag::EmptyTag;
	
	TWeakObjectPtr<ADCPlayerCharacter> PlayerWeak = nullptr;
};
