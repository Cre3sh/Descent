// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Engine/DataAsset.h>
#include <GameplayTagContainer.h>

#include "DCRoomGenerationData.generated.h"

USTRUCT(BlueprintType)
struct FDCRoomGenerationInfo
{
	GENERATED_BODY()

public:
	// The level we would like to load for this room
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UWorld> LevelToLoad;

	// The corresponding room type for this generation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Categories="Procedural.RoomType"))
	FGameplayTag RoomType = FGameplayTag::EmptyTag;

	// The amount of this type of room we would like to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AmountToSpawn = 0;
};

UCLASS()
class UDCRoomGenerationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FDCRoomGenerationInfo> RoomGenerationInfos;
};