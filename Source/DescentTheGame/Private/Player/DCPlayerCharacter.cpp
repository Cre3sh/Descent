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
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Components/WidgetComponent.h>
#include <Net/UnrealNetwork.h>

#include "DCPickupManagerComponent.h"
#include "DCPlayerState.h"
#include "Audio/DCTerrorRadiusComponent.h"
#include "Base/DCAdvancedGameInstance.h"
#include "Camera/CameraActor.h"
#include "Interactable/DCInteractableObject.h"
#include "Net/Core/PushModel/PushModel.h"
#include "UI/DCPlayerHUD.h"
#include "UI/Player/DCUIPlayerInfoWidget.h"
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

	PlayerInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerInfoWidget"));
	PlayerInfoWidgetComponent->SetupAttachment(RootComponent);

	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Scene Capture Camera"));
	SceneCaptureComponent->SetupAttachment(MinimapSpringArmComponent);

	PickupManagerComponent = CreateDefaultSubobject<UDCPickupManagerComponent>(TEXT("Pickup manager component"));
	TerrorComponent = CreateDefaultSubobject<UDCTerrorRadiusComponent>(TEXT("Terror Component"));

	SpectatorCameraHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Spectator Camera Holder"));
	SpectatorCameraHolder->SetupAttachment(PlayerCamera);

	ApparitionDeathSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("Apparition Death Sound"));

	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Foot Step Audio Component"));
	FlashlightAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Flashlight sound"));
	Torch = CreateDefaultSubobject<USpotLightComponent>(TEXT("TorchLight"));
	PawnNoiseEmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitterComp"));

	CrouchEyeOffset = FVector(0.f);
	CrouchSpeed = 6.f;
}

void ADCPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	if (bNeedsHUDSetup)
	{
		SetupPlayerHUD();
	}
}

void ADCPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (bNeedsHUDSetup)
	{
		SetupPlayerHUD();
	}
}

// Called when the game starts or when spawned
void ADCPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* const PlayerController = GetController<APlayerController>();
	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

		check(InputSubsystem);

		// Add Input Mapping Context
		InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
	}

	check(PlayerInfoWidgetComponent);
	check(FootstepAudioComponent);

	PlayerInfoWidgetComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlayerInfoWidgetComponent->AddRelativeLocation(FVector(0, 0, 100.0f));

	FootstepAudioComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	if (!IsLocallyControlled())
	{
		SpectatorCameraActor = Cast<ACameraActor>(GetWorld()->SpawnActor(ACameraActor::StaticClass()));

		SpectatorCameraActor->SetOwner(this);
		SpectatorCameraActor->SetActorLocation(SpectatorCameraHolder->GetComponentLocation());
		SpectatorCameraActor->SetActorRotation(SpectatorCameraHolder->GetComponentRotation());
		SpectatorCameraActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	// Set up local player info
	if (IsLocallyControlled())
	{
		if (IsValid(GetPlayerState()) && bNeedsHUDSetup)
		{
			SetupPlayerHUD();
		}

		const UDCAdvancedGameInstance* const GameInstance = GetGameInstance<UDCAdvancedGameInstance>();

		check(GameInstance);

		FText PlayerName = GameInstance->LocalPlayerName;

		Server_SetPlayerName(PlayerName);

		check(PlayerInfoWidgetComponent);

		PlayerInfoWidgetComponent->SetVisibility(false);
	}

	/*Make it so the flashlight follows the camera's rotation*/

	check(Torch);
	check(MinimapSpringArmComponent);
	check(SceneCaptureComponent);

	Torch->AttachToComponent(PlayerCamera, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	MinimapSpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
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

void ADCPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;

	DOREPLIFETIME_WITH_PARAMS_FAST(ADCPlayerCharacter, CustomPlayerName, Params);
	DOREPLIFETIME_WITH_PARAMS_FAST(ADCPlayerCharacter, LastInteractedObject, Params);

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

ACameraActor* ADCPlayerCharacter::GetSpectatorCamera()
{
	return SpectatorCameraActor;
}

void ADCPlayerCharacter::StartedInteractingWithObject(const FGameplayTag& GameplayTag) const
{
	// TODO change this to use a delegate

	check(PlayerInfoWidgetComponent);

	UDCUIPlayerInfoWidget* const PlayerInfoWidget = Cast<UDCUIPlayerInfoWidget>(PlayerInfoWidgetComponent->GetWidget());
	if (!IsValid(PlayerInfoWidget))
	{
		return;
	}

	PlayerInfoWidget->PlayerStartedInteraction(GameplayTag);
}

void ADCPlayerCharacter::StoppedInteractingWithObject()
{
	// TODO change this to use a delegate

	check(PlayerInfoWidgetComponent);

	UDCUIPlayerInfoWidget* const PlayerInfoWidget = Cast<UDCUIPlayerInfoWidget>(PlayerInfoWidgetComponent->GetWidget());
	if (!IsValid(PlayerInfoWidget))
	{
		return;
	}
	
	LastInteractedObject = nullptr;
	PlayerInfoWidget->PlayerStoppedInteraction();
}

void ADCPlayerCharacter::SetLastInteractedObject(ADCInteractableObject* InteractableObject)
{
	LastInteractedObject = InteractableObject;

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerCharacter, LastInteractedObject, this);
}

ADCInteractableObject* ADCPlayerCharacter::GetLastInteractedObject() const
{
	return LastInteractedObject.Get();
}

UDCPickupManagerComponent* ADCPlayerCharacter::GetPickupManagerComponent() const
{
	return PickupManagerComponent;
}

UMediaSoundComponent* ADCPlayerCharacter::GetMediaSoundComponent() const
{
	return ApparitionDeathSound;
}

UDCUISceneManager* ADCPlayerCharacter::GetSceneManager() const
{
	return SceneManager;
}

void ADCPlayerCharacter::Server_SetPlayerName_Implementation(const FText& PlayerName)
{
	CustomPlayerName = PlayerName;

	// Need to set the player info straight away for the host
	SetPlayerInfoName();

	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerCharacter, CustomPlayerName, this);
}

void ADCPlayerCharacter::Server_ReportNoise_Implementation()
{
	ReportNoise();
}

void ADCPlayerCharacter::OnPlayerCaught()
{
	GetCharacterMovement()->DisableMovement();
	DisableInput(GetController<APlayerController>());

	USkeletalMeshComponent* const SkeletalMeshComponent = GetMesh();
	
	check(SkeletalMeshComponent);

	SkeletalMeshComponent->SetSimulatePhysics(true);
	SkeletalMeshComponent->SetCollisionProfileName("Ragdoll");

	float RandomX = FMath::RandRange(25000.0f, 100000.0f);
	float RandomY = FMath::RandRange(25000.0f, 100000.0f);
	float RandomZ = FMath::RandRange(25000.0f, 100000.0f);
	SkeletalMeshComponent->AddImpulse(FVector(RandomX, RandomY, RandomZ));

	ADCPlayerState* const OwningPlayerState = GetPlayerState<ADCPlayerState>();

	check(OwningPlayerState);

	OwningPlayerState->SetPlayerDead(true);
}

// Called to bind functionality to input
void ADCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::InteractCheck);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADCPlayerCharacter::EndSprint);
	}

	// todo add back once we have a main menu
	//EnhancedInputComponent->BindAction(EscapeAction, ETriggerEvent::Completed, SceneManager.Get(), &UDCUISceneManager::OnPlayerPressedEscape);
}

void ADCPlayerCharacter::SetupPlayerHUD()
{
	APlayerController* const PlayerController = GetController<APlayerController>();
	if (!IsValid(PlayerController))
	{
		return;
	}

	if (PlayerController->IsLocalController())
	{
		PlayerHUD = CreateWidget<UDCPlayerHUD>(PlayerController, PlayerHUDClass);
		if (IsValid(PlayerHUD))
		{
			PlayerHUD->AddToViewport();
			PlayerHUD->OnOwningPlayerStateSet(GetPlayerState<ADCPlayerState>());
		}

		SceneManager = CreateWidget<UDCUISceneManager>(PlayerController, SceneManagerClass);
		if (IsValid(SceneManager))
		{
			SceneManager->AddToViewport();
		}

		bNeedsHUDSetup = false;
	}
}

void ADCPlayerCharacter::StartSprint()
{
	if (!HasAuthority())
	{
		Server_StartSprint();
	}

	CurrentPlayerMovementState = Running;

	if (bCrouching) return;

	/*The player is sprinting, so we want them to be easily audible by the AI*/

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

void ADCPlayerCharacter::SetPlayerInfoName()
{
	UDCUIPlayerInfoWidget* const PlayerInfoWidget = Cast<UDCUIPlayerInfoWidget>(PlayerInfoWidgetComponent->GetWidget());

	PlayerInfoWidget->SetTrackedPlayer(CustomPlayerName);
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

void ADCPlayerCharacter::OnRep_LastInteractedObject()
{
	const ADCInteractableObject* const InteractableObject = LastInteractedObject.Get();
	if (IsValid(InteractableObject))
	{
		StartedInteractingWithObject(InteractableObject->GetInteractionObjectTag());
	}
	else
	{
		StoppedInteractingWithObject();
	}
}

void ADCPlayerCharacter::OnRep_CustomPlayerName()
{
	check(PlayerInfoWidgetComponent);

	UDCUIPlayerInfoWidget* const PlayerInfoWidget = Cast<UDCUIPlayerInfoWidget>(PlayerInfoWidgetComponent->GetWidget());

	if (!IsValid(PlayerInfoWidget))
	{
		//todo Probably best not to rely on a hard coded value of time so DO NOT do this. No time now though so will leave it
		GetWorld()->GetTimerManager().SetTimer(WidgetRetryConstruction, this, &ADCPlayerCharacter::SetPlayerInfoName, 1.0f);
		return;
	}

	PlayerInfoWidget->SetTrackedPlayer(CustomPlayerName);
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

	bool bHas = HasAuthority();

	if (IsValid(PlayerController))
	{
		if (PlayerController->IsLocalController())
		{
			PlayerController->PlayerCameraManager->StartCameraShake(*CameraShakes.Find(CurrentPlayerMovementState));
		}
	}	
}