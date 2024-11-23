// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Weapon/DCHandgun.h"

#include "Enemy/DCMovementHaltEntity.h"
#include "Player/DCPlayerCharacter.h"

// Sets default values
ADCHandgun::ADCHandgun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HandgunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	HandgunMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADCHandgun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADCHandgun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADCHandgun::SetActive(bool bIsActive)
{
	if (!bIsActive)
	{
		HandgunMeshComponent->SetVisibility(false);
		HandgunMeshComponent->SetActive(false);
	}
	else
	{
		HandgunMeshComponent->SetVisibility(true);
		HandgunMeshComponent->SetActive(true);
	}
}

void ADCHandgun::Shoot_Implementation()
{
	HandgunMeshComponent->PlayAnimation(ShootAnimation, false);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	TArray<AActor*> IgnoredActors = {this, GetOwner()};

	QueryParams.AddIgnoredActors(IgnoredActors);
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorForwardVector() * 500.0f, ECollisionChannel::ECC_Pawn, QueryParams);

	if (ADCPlayerCharacter* PlayerCharacter = Cast<ADCPlayerCharacter>(Hit.GetActor()))
	{
		PlayerCharacter->OnPlayerCaught(Tag_Enemy_Cessare);
	}
}
