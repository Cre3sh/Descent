// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>

#include "DCTerrorRadiusComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UDCTerrorRadiusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDCTerrorRadiusComponent();
	
	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor override

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> TerrorAudio = nullptr;

private:
	UFUNCTION()
	void OnAudioFinished();
	
	void CheckForNearbyPlayers();

	UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;

	FTimerHandle CheckPlayerHandle;

	bool bIsPlaying = false;
};
