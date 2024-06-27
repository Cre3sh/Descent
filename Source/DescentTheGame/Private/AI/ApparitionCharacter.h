// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Character.h>

#include "ApparitionCharacter.generated.h"

class UPawnSensingComponent;
class UBehaviorTree;
class UBoxComponent;

UCLASS()
class DESCENTTHEGAME_API AApparitionCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AApparitionCharacter();
	
	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// End AActor override

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = AI)
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> PlayerDetectionBoxComponent = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> StaticObjectBoxComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = AI)
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> TerrorSoundComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> FootStepSoundComponent = nullptr;

	UPROPERTY(BlueprintReadWrite)
	bool PlayerDead = false;

private:
	/*Hearing function - will be executed when we hear a Pawn*/
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UFUNCTION()
	void OnPlayerCaught(APawn* InPawn);

	// Callback exclusively for hitting pawns
	UFUNCTION()
	void OnPawnDetecionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Callbacks for static objects
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
