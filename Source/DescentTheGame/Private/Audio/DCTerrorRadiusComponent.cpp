// Fill out your copyright notice in the Description page of Project Settings.


#include "Audio/DCTerrorRadiusComponent.h"

// Sets default values for this component's properties
UDCTerrorRadiusComponent::UDCTerrorRadiusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDCTerrorRadiusComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDCTerrorRadiusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

