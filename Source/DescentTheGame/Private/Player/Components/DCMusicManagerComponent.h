// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>

#include "DCMusicManagerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDCMusicManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDCMusicManagerComponent();

	// Begin UActorComponent override
	virtual void BeginPlay() override;
	// End UActorComponent override

	void StartChase() const;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundBase> ChaseMusic = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> ChaseAudioComponent = nullptr;
};