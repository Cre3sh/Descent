// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Character.h>

#include "DCMenuCharacter.generated.h"

UCLASS()
class ADCMenuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADCMenuCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> MainMenuWidgetClass = nullptr;
};
