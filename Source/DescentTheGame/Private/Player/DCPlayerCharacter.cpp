// Fill out your copyright notice in the Description page of Project Settings.

#include "DCPlayerCharacter.h"

#include <Camera/CameraComponent.h>
#include <Components/InputComponent.h>
#include <Math/UnrealMathUtility.h>
#include <Components/SpotLightComponent.h>
#include <Components/AudioComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/PawnNoiseEmitterComponent.h>
#include <Engine/StreamableManager.h>
#include <Engine/AssetManager.h>
#include <Components/SceneCaptureComponent2D.h>
#include <GameFramework/SpringArmComponent.h>
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include "Interactable/DCInteractableObject.h"
#include "UI/DCPlayerHUD.h"
#include "UI/Systems/DCUISceneManager.h"

// Sets default values
ADCPlayerCharacter::ADCPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(RootComponent);
	PlayerCamera->SetRelativeLocation(FVector(0, 0, 40));
	PlayerCamera->bUsePawnControlRotation = true;

	MinimapSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Minimap Spring Arm"));

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Camera"));
	SceneCaptureComponent->SetupAttachment(MinimapSpringArmComponent);
	ApparitionDeathSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Apparition Death Sound"));

	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Foot Step Audio Component"));
	FlashlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Flashlight sound"));
	Torch = CreateDefaultSubobject<USpotLightComponent>(TEXT("TorchLight"));
	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 6.f;
}

// Called when the game starts or when spawned
void ADCPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}

	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (IsValid(PlayerController))
	{
		if (PlayerController->IsLocalController())
		{
			PlayerHUD = CreateWidget<UDCPlayerHUD>(PlayerController, PlayerHUDClass);
			if (IsValid(PlayerHUD))
			{
				PlayerHUD->AddToViewport();
			}

			SceneManager = CreateWidget<UDCUISceneManager>(UGameplayStatics::GetPlayerController(this, 0), SceneManagerClass);
			if (IsValid(SceneManager))
			{
				SceneManager->AddToViewport();
			}
		}
	}

	/*Make it so the flash light follows the camera's rotation*/

	check(Torch);
	check(MinimapSpringArmComponent);
	check(SceneCaptureComponent);

	Torch->AttachToComponent(PlayerCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MinimapSpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AActor::StaticClass(), FName("Ceiling"), Actors);

	for (const AActor* const Actor : Actors)
	{
		check(Actor);
		SceneCaptureComponent->HideComponent(Actor->GetComponentByClass<UStaticMeshComponent>());
	}
}

// Called every frame
void ADCPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFootsteps(DeltaTime);
	UpdateMovementState();
	UpdateCamera();

	const float CrouchInterpTime = FMath::Min(1.f, CrouchSpeed * DeltaTime);
	CrouchEyeOffset = (1.f - CrouchInterpTime) * CrouchEyeOffset;
}

void ADCPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	LoadSoundFiles();
}

ADCInteractableObject* ADCPlayerCharacter::GetLastInteractedObject() const
{
	return LastInteractedObject.Get();
}

UMediaSoundComponent* ADCPlayerCharacter::GetMediaSoundComponent() const
{
	return ApparitionDeathSound;
}

UDCUISceneManager* ADCPlayerCharacter::GetSceneManager() const
{
	return SceneManager;
}

void ADCPlayerCharacter::Server_ReportNoise_Implementation()
{
	ReportNoise();
}

void ADCPlayerCharacter::KillPlayer(ADCPlayerCharacter* PlayerCharacter) const
{
	if (!IsValid(PlayerCharacter))
	{
		return;
	}

	if (PlayerCharacter->IsLocallyControlled())
	{
		PlayerHUD->OnPlayerDied();
	}

	USkeletalMeshComponent* const SkeletalMeshComponent = GetMesh();
	
	check(SkeletalMeshComponent);

	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->SetCollisionProfileName("Ragdoll");

	float RandomX = FMath::RandRange(25000.0f, 100000.0f);
	float RandomY = FMath::RandRange(25000.0f, 100000.0f);
	float RandomZ = FMath::RandRange(25000.0f, 100000.0f);
	SkeletalMeshComponent->AddImpulse(FVector(RandomX, RandomY, RandomZ));
}

// Called to bind functionality to input
void ADCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Look);
	}

	InputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ADCPlayerCharacter::InteractCheck);
	InputComponent->BindAction(TEXT("Crouch"), IE_Pressed, this, &ADCPlayerCharacter::StartCrouch);
	InputComponent->BindAction(TEXT("Crouch"), IE_Released, this, &ADCPlayerCharacter::EndCrouch);
	InputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &ADCPlayerCharacter::StartSprint);
	InputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &ADCPlayerCharacter::EndSprint);

	// todo add back once we have a main menu
	//InputComponent->BindAction(TEXT("Escape"), IE_Pressed, SceneManager.Get(), &UDCUISceneManager::OnPlayerPressedEscape);
}

void ADCPlayerCharacter::StartSprint()
{
	if (!HasAuthority())
	{
		Server_StartSprint();
	}

	CurrentPlayerMovementState = Running;

	if (bCrouching) return;

	/*The player is sprinting so we want them to be easily audible by the AI*/

	PlayerLoudness = 1.0f;

	bSprinting = true;

	UCharacterMovementComponent* const CharacterMovementComponent = GetCharacterMovement();

	check(CharacterMovementComponent);

	CharacterMovementComponent->MaxWalkSpeed = RunSpeed;
}

void ADCPlayerCharacter::Server_EndSprint_Implementation()
{
	EndSprint();
}

void ADCPlayerCharacter::EndSprint()
{
	if (!HasAuthority())
	{
		Server_EndSprint();
	}

	CurrentPlayerMovementState = Walking;

	bSprinting = false;

	PlayerLoudness = 0.25f;

	UCharacterMovementComponent* const CharacterMovementComponent = GetCharacterMovement();

	check(CharacterMovementComponent);

	CharacterMovementComponent->MaxWalkSpeed = WalkSpeed;
}

void ADCPlayerCharacter::ToggleLight() const
{
	if (!HasFlashlight) return;

	Torch->ToggleVisibility();
	FlashlightAudioComponent->Play();
}

void ADCPlayerCharacter::LoadSoundFiles()
{
	FStreamableManager& BaseLoader = UAssetManager::GetStreamableManager();
	TArray<FSoftObjectPath> AudioSoftObjectPaths = {};

	for (const TSoftObjectPtr<USoundBase>& SoundBase : FootstepSounds)
	{
		const TSoftObjectPtr<USoundBase> AudioSoftObjectPtr = SoundBase;

		AudioSoftObjectPaths.Add(AudioSoftObjectPtr.ToSoftObjectPath());
	}

	BaseLoader.RequestAsyncLoad(AudioSoftObjectPaths);
}

void ADCPlayerCharacter::CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult)
{
	if (PlayerCamera)
	{
		PlayerCamera->GetCameraView(DeltaTime, OutResult);
		OutResult.Location += CrouchEyeOffset;
	}
}

void ADCPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADCPlayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADCPlayerCharacter::ReportNoise()
{
	//Report that we've played a sound with a certain volume in a specific location

	if (!HasAuthority())
	{
		Server_ReportNoise();
		return;
	}

	MakeNoise(PlayerLoudness, this, GetActorLocation());
}

void ADCPlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f)
	{
		return;
	}

	EndSprint();

	const float StartBaseEyeHeight = BaseEyeHeight;

	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight + HalfHeightAdjust;

	PlayerCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);

	PlayerLoudness = 0.05f;
}

void ADCPlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if (HalfHeightAdjust == 0.f)
	{
		return;
	}

	const float StartBaseEyeHeight = BaseEyeHeight;

	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	CrouchEyeOffset.Z += StartBaseEyeHeight - BaseEyeHeight - HalfHeightAdjust;

	PlayerCamera->SetRelativeLocation(FVector(0.f, 0.f, BaseEyeHeight), false);
}

void ADCPlayerCharacter::FootCheck()
{
	FHitResult Hit;
	const FVector PlayerLocation = GetActorLocation();
	FVector EndLocation = PlayerLocation;
	EndLocation.Z -= 150.f;
	const FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerLocation, EndLocation, ECC_Visibility, TraceParams);

	if (IsValid(Hit.GetActor()))
	{
		const int32 RandomIndex = FMath::RandRange(0, FootstepSounds.Num() - 1);

		if (FootstepSounds.IsValidIndex(RandomIndex))
		{
			check(FootstepAudioComponent);

			USoundBase* SoundFile = FootstepSounds[RandomIndex].Get();
			FootstepAudioComponent->SetSound(SoundFile);
			FootstepAudioComponent->Play();

			ReportNoise();
		}
	}
}

void ADCPlayerCharacter::Server_InteractCheck_Implementation()
{
	InteractCheck();
}

void ADCPlayerCharacter::StartCrouch()
{
	CurrentPlayerMovementState = Crouching;

	bCrouching = true;

	bSprinting = false;

	Crouch();
	PlayerLoudness = 0.05f;
}

void ADCPlayerCharacter::EndCrouch()
{
	FootstepAudioComponent->VolumeMultiplier = 0.55f;
	PlayerLoudness = 0.25f;
	bCrouching = false;
	UnCrouch();
}

void ADCPlayerCharacter::Server_StartSprint_Implementation()
{
	StartSprint();
}

void ADCPlayerCharacter::InteractCheck()
{
	if (!HasAuthority())
	{
		Server_InteractCheck();
	}

	const UWorld* const World = GetWorld();
	check(World);

	FHitResult hit;
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	const FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	const FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	World->LineTraceSingleByChannel(hit, PlayerViewPointLocation, LineTraceEnd, ECC_Visibility, TraceParams);

	AActor* const ActorHit = hit.GetActor();

	if (ActorHit)
	{
		ADCInteractableObject* const InteractableObject = Cast<ADCInteractableObject>(ActorHit);
		if (IsValid(InteractableObject))
		{
			InteractableObject->Interact(this);

			LastInteractedObject = InteractableObject;
		}
	}
}

void ADCPlayerCharacter::UpdateFootsteps(const float DeltaTime)
{
	DistanceTravelled += GetVelocity().Size() * DeltaTime;

	if (DistanceTravelled >= 100.0f)
	{
		DistanceTravelled = 0.0f;

		FootCheck();
	}
}

void ADCPlayerCharacter::Server_UpdateMovementState_Implementation()
{
	UpdateMovementState();
}

void ADCPlayerCharacter::UpdateMovementState()
{
	const float Velocity = GetVelocity().Size();
	if (FMath::IsNearlyZero(Velocity, 1.0f))
	{
		CurrentPlayerMovementState = Idle;
		return;
	}

	// Refresh movement state
	if (bSprinting)
	{
		CurrentPlayerMovementState = Running;
		return;
	}

	if (bCrouching)
	{
		CurrentPlayerMovementState = Crouching;
		return;
	}

	CurrentPlayerMovementState = Walking;
}

void ADCPlayerCharacter::UpdateCamera()
{
	const APlayerController* const PlayerController = Cast<APlayerController>(GetController());

	if (IsValid(PlayerController))
	{
		if (PlayerController->IsLocalController())
		{
			PlayerController->PlayerCameraManager->StartCameraShake(*CameraShakes.Find(CurrentPlayerMovementState));
		}
	}	
}