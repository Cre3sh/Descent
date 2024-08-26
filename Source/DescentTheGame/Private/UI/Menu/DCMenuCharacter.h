// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Character.h>

#include "DCMenuCharacter.generated.h"

class UDCUISceneManager;

UCLASS()
class ADCMenuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADCMenuCharacter();

	// Begin AActor override
	virtual void BeginPlay() override;
	// End AActor override

	UPROPERTY()
	TObjectPtr<UDCUISceneManager> SceneManager = nullptr;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SceneManagerWidgetClass = nullptr;
};
