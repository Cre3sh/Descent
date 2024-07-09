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
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor override

	UFUNCTION(Server, Reliable)
	void Server_UnlockDoor();
	void UnlockDoor();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> DoorMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AccessAudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAudioComponent> AudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> DoorOpenSound = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> DoorCloseSound = nullptr;

	UPROPERTY(EditAnywhere)
	bool bShouldStartLocked = false;

	UPROPERTY(EditDefaultsOnly)
	float DoorOpenZ = 268.0f;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OpenDoor();
	
	void CheckShouldClose();

	void SetWantsToOpen(bool bInWantsToOpen);
	void SetWantsToClose(bool bInWantsToClose);

	TArray<TWeakObjectPtr<APawn>> OverlappedWeakPawns;

	FVector CurrentLocation = FVector::ZeroVector;

	FVector DoorOpenPosition = FVector::ZeroVector;
	FVector DoorClosePosition = FVector::ZeroVector;

	FTimerHandle CloseCheckHandle;

	UPROPERTY(Replicated)
	bool bIsLocked = false;

	UPROPERTY(Replicated)
	bool bWantsToOpen = false;

	UPROPERTY(Replicated)
	bool bWantsToClose = false;
};