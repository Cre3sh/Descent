// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "DCUsableIconData.generated.h"

UCLASS()
class UDCUsableIconData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, UTexture2D*> IconMap;
};
