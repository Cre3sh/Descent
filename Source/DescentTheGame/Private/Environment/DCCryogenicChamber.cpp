// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/DCCryogenicChamber.h"

// Sets default values
ADCCryogenicChamber::ADCCryogenicChamber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerHolder = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerHolder"));
	PlayerHolder->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADCCryogenicChamber::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADCCryogenicChamber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

