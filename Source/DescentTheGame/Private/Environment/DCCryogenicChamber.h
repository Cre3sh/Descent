// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "DCCryogenicChamber.generated.h"

UCLASS()
class ADCCryogenicChamber : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADCCryogenicChamber();

	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// End AActor override

	UFUNCTION(Server, Reliable)
	void Server_ReviveRandomPlayer();
	void ReviveRandomPlayer();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> CryogenicStaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> GlassStaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerHolder = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> SmokeParticleSystem = nullptr;

private:
	void BeginRevive();

	void RevivePlayer();
	
	FVector CurrentGlassLocation = FVector::ZeroVector;
	FVector GlassOpenPosition = FVector::ZeroVector;

	bool bIsUsable = true;

	bool bIsMoving = false;

	FTimerHandle RevivePlayerHandle;
};
