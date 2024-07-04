// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "DCSlidingDoor.generated.h"

class UBoxComponent;

UCLASS()
class ADCSlidingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADCSlidingDoor();
	
	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor override

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> DoorMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> DoorOpenSound = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> DoorCloseSound = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float DoorOpenZ = 268.0f;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void CheckShouldClose();

	TArray<TWeakObjectPtr<APawn>> OverlappedWeakPawns;

	FVector CurrentLocation = FVector::ZeroVector;

	FVector DoorOpenPosition = FVector::ZeroVector;
	FVector DoorClosePosition = FVector::ZeroVector;

	FTimerHandle CloseCheckHandle;

	bool bWantsToOpen = false;
	bool bWantsToClose = false;
};