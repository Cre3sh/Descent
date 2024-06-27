// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>

#include "DCSceneData.generated.h"

struct FGameplayTag;

UCLASS()
class UDCSceneData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta=(ForceInlineRow))
	TMap<FGameplayTag, TSubclassOf<UUserWidget>> ScenesMap;
};
