// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "DCUIPlayerInteractionInfoData.generated.h"

UCLASS()
class UDCUIPlayerInteractionInfoData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Materials", meta=(ForceInlineRow))
	TMap<FGameplayTag, UMaterialInstance*> InteractionMaterials;
};
