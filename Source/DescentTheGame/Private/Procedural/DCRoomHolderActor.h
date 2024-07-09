// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "GameplayTagContainer.h"
#include "DCRoomHolderActor.generated.h"

struct FGameplayTag;

UCLASS()
class ADCRoomHolderActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADCRoomHolderActor();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRootComponent = nullptr;

	UPROPERTY(EditAnywhere, meta=(Categories = "Procedural.RoomType"))
	FGameplayTag SupportedRoomType = FGameplayTag::EmptyTag;
};
