// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> PlayerHolder = nullptr;
};
