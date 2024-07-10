// Fill out your copyright notice in the Description page of Project Settings.

#include "Environment/DCCryogenicChamber.h"

#include <Components/AudioComponent.h>
#include <GameFramework/GameModeBase.h>
#include <GameFramework/GameStateBase.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

#include "Player/DCPlayerState.h"

// Sets default values
ADCCryogenicChamber::ADCCryogenicChamber()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CryogenicStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CryogenicMesh"));
	SetRootComponent(CryogenicStaticMesh);

	GlassStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GlassStaticMesh"));
	GlassStaticMesh->SetupAttachment(CryogenicStaticMesh);

	PlayerHolder = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerHolder"));
	PlayerHolder->SetupAttachment(CryogenicStaticMesh);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(CryogenicStaticMesh);
}

// Called when the game starts or when spawned
void ADCCryogenicChamber::BeginPlay()
{
	Super::BeginPlay();

	check(AudioComponent);

	AudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CurrentGlassLocation = GlassStaticMesh->GetRelativeLocation();

	GlassOpenPosition = FVector(CurrentGlassLocation.X, CurrentGlassLocation.Y, CurrentGlassLocation.Z - 250.0f);
}

// Called every frame
void ADCCryogenicChamber::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		CurrentGlassLocation = FMath::VInterpConstantTo(CurrentGlassLocation, GlassOpenPosition, DeltaTime, 50.0f);
		GlassStaticMesh->SetRelativeLocation(CurrentGlassLocation);

		if (CurrentGlassLocation.Equals(GlassOpenPosition))
		{
			bIsMoving = false;
		}
	}
}

void ADCCryogenicChamber::Server_ReviveRandomPlayer_Implementation()
{
	ReviveRandomPlayer();
}

void ADCCryogenicChamber::ReviveRandomPlayer()
{
	if (!HasAuthority())
	{
		Server_ReviveRandomPlayer();
	}

	if (!bIsUsable)
	{
		return;
	}

	bIsUsable = false;

	AudioComponent->Play();

	// Set a timer for 10 seconds after which we open the door and revive our player
	GetWorld()->GetTimerManager().SetTimer(RevivePlayerHandle, this, &ADCCryogenicChamber::BeginRevive, 10.0f, false);
}

void ADCCryogenicChamber::BeginRevive()
{
	RevivePlayer();

	UParticleSystemComponent* const ParticleSystemComponent = UGameplayStatics::SpawnEmitterAtLocation(this, SmokeParticleSystem, PlayerHolder->GetComponentLocation(), PlayerHolder->GetComponentRotation(), FVector(5.0f, 5.0f, 5.0f));
	bIsMoving = true;

	GetWorld()->GetTimerManager().SetTimer(DestroySmokeHandle, FTimerDelegate::CreateWeakLambda(this, [this, ParticleSystemComponent]()
	{
		if (IsValid(ParticleSystemComponent))
		{
			ParticleSystemComponent->DeactivateSystem();
		}
	}), 15.0f, false);
}

void ADCCryogenicChamber::RevivePlayer()
{
	const UWorld* const World = GetWorld();
	check(World);

	AGameModeBase* const GameModeBase = World->GetAuthGameMode();
	if (!IsValid(GameModeBase))
	{
		return;
	}

	AGameStateBase* const GameStateBase = World->GetGameState();
	if (!IsValid(GameStateBase))
	{
		return;
	}

	for (APlayerState* const PlayerState : GameStateBase->PlayerArray)
	{
		ADCPlayerState* const DCPlayerState = Cast<ADCPlayerState>(PlayerState);

		check(DCPlayerState);

		if (DCPlayerState->IsPlayerDead())
		{
			FTransform PlayerHolderTransform = PlayerHolder->GetComponentTransform();
			PlayerHolderTransform.SetScale3D(FVector(1.0f, 1.0f, 1.0f));
			AController* PlayerController = DCPlayerState->GetOwningController();

			GameModeBase->RestartPlayerAtTransform(PlayerController, PlayerHolderTransform);

			break;
		}
	}
}