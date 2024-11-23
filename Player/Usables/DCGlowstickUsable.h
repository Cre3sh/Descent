// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Player/Inventory/DCUsableItem.h"

#include "DCGlowstickUsable.generated.h"

UCLASS()
class ADCGlowstickUsable : public ADCUsableItem
{
	GENERATED_BODY()

public:
	ADCGlowstickUsable();

	virtual void Use() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> UseSound = nullptr;
};
