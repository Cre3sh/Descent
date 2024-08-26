// Fill out your copyright notice in the Description page of Project Settings.

#include "ApparitionCharacter.h"

#include <Runtime/Engine/Classes/Engine/World.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/PlayerController.h>
#include <Perception/PawnSensingComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Components/AudioComponent.h>
#include <Math/Vector.h>
#include <Components/BoxComponent.h>

#include "Player/DCPlayerCharacter.h"
#include "ApparitionAIController.h"
#include "Environment/DCFlickeringLight.h"

// Sets default values
AApparitionCharacter::AApparitionCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	check(PawnSensingComp);

	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	TerrorSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TerrorSound"));

	FootStepSoundComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FootStepSound"));

	PlayerDetectionBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Player Detection Box Component"));

	StaticObjectBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Static Object Box Component"));
}

// Called when the game starts or when spawned
void AApparitionCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(PawnSensingComp);
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AApparitionCharacter::OnPlayerCaught);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AApparitionCharacter::OnHearNoise);

	check(PlayerDetectionBoxComponent);
	PlayerDetectionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AApparitionCharacter::OnPawnDetectionBeginOverlap);

	PlayerDetectionBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	check(StaticObjectBoxComponent);
	StaticObjectBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AApparitionCharacter::OnComponentBeginOverlap);
	StaticObjectBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AApparitionCharacter::OnComponentEndOverlap);

	StaticObjectBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	check(FootStepSoundComponent);

	FootStepSoundComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AApparitionCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	check(PlayerDetectionBoxComponent);
	check(StaticObjectBoxComponent);

	PlayerDetectionBoxComponent->OnComponentBeginOverlap.RemoveAll(this);
	StaticObjectBoxComponent->OnComponentBeginOverlap.RemoveAll(this);
	StaticObjectBoxComponent->OnComponentEndOverlap.RemoveAll(this);
}

void AApparitionCharacter::Tick(float DeltaTime)
{
	check(FootStepSoundComponent);

	if (!FootStepSoundComponent->IsPlaying() && !PlayerDead && !WeakCaughtCharacter.IsValid())
	{
		FootStepSoundComponent->Play();
	}

	ADCPlayerCharacter* const PlayerCharacter = WeakCaughtCharacter.Get();

	AApparitionAIController* const AIController = Cast<AApparitionAIController>(GetController());

	check(AIController);
	if (IsValid(PlayerCharacter) && PlayerCharacter->IsHiding())
	{
		WeakCaughtCharacter.Reset();

		AIController->SetPlayerCaught(nullptr);
	}
}

void AApparitionCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	AApparitionAIController* const AIController = Cast<AApparitionAIController>(GetController());

	check(AIController);

	//We don't want to hear ourselves
	if (PawnInstigator != this)
	{
		//Updates our target based on what we've heard.
		AIController->SetSensedTarget(PawnInstigator);
		AIController->SetSoundLocation(Location);
	}
}

void AApparitionCharacter::OnPlayerCaught(APawn* InPawn)
{
	ADCPlayerCharacter* const PlayerCharacter = Cast<ADCPlayerCharacter>(InPawn);

	check(PlayerCharacter);

	if (PlayerCharacter->IsHiding())
	{
		return;
	}

	AApparitionAIController* const AIController = Cast<AApparitionAIController>(GetController());

	check(AIController);

	if (AIController)
	{
		AIController->SetPlayerCaught(InPawn);
	}
}

void AApparitionCharacter::OnPawnDetectionBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADCPlayerCharacter* const PlayerCharacter = Cast<ADCPlayerCharacter>(Other);
	if (!IsValid(PlayerCharacter) || PlayerCharacter->IsHiding())
	{
		return;
	}

	WeakCaughtCharacter = PlayerCharacter;

	FootStepSoundComponent->Stop();

	PlayerCharacter->OnPlayerCaught();
}

void AApparitionCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADCFlickeringLight* const FlickeringLight = Cast<ADCFlickeringLight>(Other);
	if (IsValid(FlickeringLight))
	{
		FlickeringLight->SetFlickerActive(true);
	}
}

void AApparitionCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ADCFlickeringLight* const FlickeringLight = Cast<ADCFlickeringLight>(OtherActor);
	if (IsValid(FlickeringLight))
	{
		FlickeringLight->SetFlickerActive(false);
	}
}

