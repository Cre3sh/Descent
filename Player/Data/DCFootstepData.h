// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>

#include "DCFootstepData.generated.h"

USTRUCT(BlueprintType)
struct FDCFootstepSounds
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> FootstepSounds;
};

UCLASS()
class UDCFootstepData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, FDCFootstepSounds> FootstepsMap;
};