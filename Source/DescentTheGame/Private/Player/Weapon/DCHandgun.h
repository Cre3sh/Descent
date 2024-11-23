// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>

#include "DCHandgun.generated.h"

UCLASS()
class ADCHandgun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADCHandgun();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetActive(bool bIsActive);

	UFUNCTION(NetMulticast, Reliable)
	void Shoot();

private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USkeletalMeshComponent> HandgunMeshComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimationAsset> ShootAnimation = nullptr;
};
