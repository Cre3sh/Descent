// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "DCUsableItem.generated.h"

UCLASS()
class ADCUsableItem : public AActor
{
	GENERATED_BODY()

public:
	virtual void Use();
};