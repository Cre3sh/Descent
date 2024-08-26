// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/GameMode.h>

#include "DCGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDCOnLevelsLoaded, UDCRoomGenerationData*, RoomGenerationData);

class UDCRoomGenerationData;

UCLASS()
class ADCGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADCGameMode();

	// Begin AGameModeBase override
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual bool ShouldSpawnAtStartSpot(AController* Player) override;
	// End AGameModeBase override

private:
	UFUNCTION()
	void OnLevelLoaded();

	void AsyncLoadLevels();
	
	void OnLevelsLoaded();

	void RestartAllPlayers();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDCRoomGenerationData> LevelInstancesData = nullptr;

	TArray<TWeakObjectPtr<AController>> PendingPlayers;

	FTimerHandle PlayerSpawnHandle;
};