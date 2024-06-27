// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"

#include <Components/StaticMeshComponent.h>
#include <Components/AudioComponent.h>
#include <Kismet/KismetMathLibrary.h>

#include "Player/DCPlayerCharacter.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetIsReplicated(true);

	DoorOpen = CreateDefaultSubobject<UAudioComponent>(TEXT("Door Open Sound"));

	DoorClose = CreateDefaultSubobject<UAudioComponent>(TEXT("Door Close Sound"));

	DoorLock = CreateDefaultSubobject<UAudioComponent>(TEXT("Door Locked Sound"));
}

void ADoor::Interact(ADCPlayerCharacter* InteractingCharacter)
{
	if (!IsValid(InteractingCharacter))
	{
		return;
	}

	FVector DirectionVector = InteractingCharacter->GetActorLocation() - GetActorLocation();
	DirectionVector = UKismetMathLibrary::LessLess_VectorRotator(DirectionVector, GetActorRotation());

	const float RotationValue = DirectionVector.X;

	if (bMoving) return;
	if (bLocked)
	{
		if (!DoorLock->IsPlaying())
		{
			DoorLock->Play();
		}
		return;
	}
	
	if (bOpen)
	{
		Multiplier *= -1;
		bMoving = true;
		UE_LOG(LogTemp, Warning, TEXT("Door closing"));
	}
	else
	{
		Multiplier = 1;
		if (RotationValue <= 0)
		{
			Multiplier *= -1;
		}

		else Multiplier *= 1;
		bMoving = true;
		UE_LOG(LogTemp, Warning, TEXT("Door Opening"));
		DoorOpen->Play();
	}

	CurrentYaw = GetActorRotation().Yaw;
	TargetYaw = CurrentYaw - (90.f * Multiplier);
	GetWorldTimerManager().SetTimer(DoorTimerHandle, FTimerDelegate::CreateLambda([this]
	{
		bMoving = false;
		if (bOpen)
		{
			DoorClose->Play();
		}
		bOpen = !bOpen;
	}), 1.5f, false);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bMoving)
	{
		CurrentYaw = FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, 4.f);
		FRotator DoorRotation = GetActorRotation();
		DoorRotation.Yaw = CurrentYaw;
		SetActorRotation(DoorRotation);
	}
}
