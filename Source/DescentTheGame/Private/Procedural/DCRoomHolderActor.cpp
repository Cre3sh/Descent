// Fill out your copyright notice in the Description page of Project Settings.

#include "Procedural/DCRoomHolderActor.h"

ADCRoomHolderActor::ADCRoomHolderActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Root Component"));

	SetRootComponent(SceneRootComponent);
}