// Fill out your copyright notice in the Description page of Project Settings.

#include "Base/DCGameMode.h"

#include <Engine/StreamableManager.h>
#include <Engine/AssetManager.h>
#include <Engine/LevelStreamingDynamic.h>
#include <EngineUtils.h>

#include "Procedural/DCRoomHolderActor.h"
#include "Procedural/Data/DCRoomGenerationData.h"

void ADCGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	AsyncLoadLevels();
}

void ADCGameMode::OnPostLogin(AController* NewPlayer)
{
	PendingPlayers.Add(NewPlayer);

	Super::OnPostLogin(NewPlayer);
}

bool ADCGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	return false;
}

void ADCGameMode::OnLevelLoaded()
{
	RestartAllPlayers();
}

void ADCGameMode::AsyncLoadLevels()
{
	FStreamableManager& BaseLoader = UAssetManager::GetStreamableManager();
	TArray<FSoftObjectPath> RoomSoftObjectPaths = {};

	check(LevelInstancesData);

	const TArray<TSoftObjectPtr<UWorld>> RoomSoftObjectPtrs;
	for (const FDCRoomGenerationInfo& RoomGenerationInfo : LevelInstancesData->RoomGenerationInfos)
	{
		RoomSoftObjectPaths.Add(RoomGenerationInfo.LevelToLoad.ToSoftObjectPath());
	}

	BaseLoader.RequestAsyncLoad(RoomSoftObjectPaths, FStreamableDelegate::CreateUObject(this, &ADCGameMode::OnLevelsLoaded));
}

void ADCGameMode::OnLevelsLoaded()
{
	bool bSuccessfulInstanceCreation = false;

	TArray<ADCRoomHolderActor*> RoomHolders;
	for (TActorIterator<ADCRoomHolderActor> It(GetWorld()); It; ++It)
	{
		ADCRoomHolderActor* const RoomHolderActor = *It;

		check(RoomHolderActor);

		RoomHolders.Add(RoomHolderActor);
	}

	for (ADCRoomHolderActor* const RoomHolderActor : RoomHolders)
	{
		ULevelStreamingDynamic* const NewRoom = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(this, LevelInstancesData->RoomGenerationInfos[0].LevelToLoad, RoomHolderActor->GetTransform(), bSuccessfulInstanceCreation);
		NewRoom->SetShouldBeLoaded(true);
		NewRoom->SetShouldBeVisible(true);

		NewRoom->OnLevelShown.AddDynamic(this, &ADCGameMode::OnLevelLoaded);

		GetWorld()->FlushLevelStreaming();
	}
}

void ADCGameMode::RestartAllPlayers()
{
	for (TWeakObjectPtr<AController> PendingPlayer : PendingPlayers)
	{
		AController* const PlayerController = PendingPlayer.Get();
		if (IsValid(PlayerController))
		{
			AActor* StartSpot = FindPlayerStart(PlayerController);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *StartSpot->GetName());
			RestartPlayer(PlayerController);
		}
	}
}