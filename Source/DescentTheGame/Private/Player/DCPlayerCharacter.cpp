// Fill out your copyright notice in the Description page of Project Settings.

#include "DCPlayerCharacter.h"

#include <Camera/CameraComponent.h>
#include <Components/InputComponent.h>
#include <Math/UnrealMathUtility.h>
#include <Components/AudioComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Engine/StreamableManager.h>
#include <Engine/AssetManager.h>
#include <GameFramework/SpringArmComponent.h>
#include <Blueprint/UserWidget.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Components/WidgetComponent.h>
#include <Net/UnrealNetwork.h>
#include <Camera/CameraActor.h>
#include <Components/BoxComponent.h>

#include "DCPickupManagerComponent.h"
#include "DCPlayerState.h"
#include "Base/DCAdvancedGameInstance.h"
#include "Components/DCMusicManagerComponent.h"
#include "Data/DCFootstepData.h"
#include "Interactable/DCInteractableObject.h"
#include "Inventory/DCInventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/Core/PushModel/PushModel.h"
#include "UI/DCPlayerHUD.h"
#include "UI/Player/DCUIPlayerInfoWidget.h"
#include "UI/Systems/DCUISceneManager.h"
#include "Utility/DCFlashlightComponent.h"
#include "Weapon/DCHandgun.h"

// Sets default values
ADCPlayerCharacter::ADCPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("TPSSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	PlayerCamera->SetupAttachment(SpringArmComponent);
	PlayerCamera->SetRelativeLocation(FVector(0, 0, 40));
	PlayerCamera->bUsePawnControlRotation = true;

	PlayerArm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Arm"));
	PlayerArm->SetupAttachment(PlayerCamera);

	PlayerBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Player Body"));
	PlayerBody->SetupAttachment(RootComponent);

	PlayerInfoWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("PlayerInfoWidget"));
	PlayerInfoWidgetComponent->SetupAttachment(RootComponent);

	InventoryComponent = CreateDefaultSubobject<UDCInventoryComponent>(TEXT("Inventory Component"));
	
	PickupManagerComponent = CreateDefaultSubobject<UDCPickupManagerComponent>(TEXT("Pickup manager component"));

	SpectatorCameraHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Spectator Camera Holder"));
	SpectatorCameraHolder->SetupAttachment(PlayerCamera);

	DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Detection Box"));
	DetectionBox->SetupAttachment(RootComponent);

	MusicManagerComponent = CreateDefaultSubobject<UDCMusicManagerComponent>(TEXT("Music Manager Component"));

	FootstepAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Foot Step Audio Component"));

	FlashlightSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Flashlight Spring Arm"));
	FlashlightComponent = CreateDefaultSubobject<UDCFlashlightComponent>(TEXT("Flashlight"));
	FlashlightComponent->SetupAttachment(FlashlightSpringArmComponent);

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

		if (IsValid(InputSubsystem))
		{
			// Add Input Mapping Context
			InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
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
		SpectatorCameraActor->bAlwaysRelevant = true;
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

		check(DetectionBox);

		SpringArmComponent->SetActive(false);

		DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ADCPlayerCharacter::OnComponentBeginOverlap);
		DetectionBox->OnComponentEndOverlap.AddDynamic(this, &ADCPlayerCharacter::OnComponentEndOverlap);
	}

	Handgun = GetWorld()->SpawnActor<ADCHandgun>(HandgunClass);
	Handgun->SetActorRelativeLocation(FVector(-5.923281, -10.940239, -2.993598));
	Handgun->SetActorRelativeRotation(FRotator(-23.774762, 242.773116, -248.750873));
	Handgun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Handgun->SetOwner(this);

	check(FlashlightSpringArmComponent);

	FlashlightSpringArmComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ADCPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (IsValid(SpectatorCameraActor))
	{
		SpectatorCameraActor->Destroy();
	}
}

// Called every frame
void ADCPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMovementState();
	UpdateCamera();

	if (IsLocallyControlled())
	{
		CheckForItem();
	}

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
	DOREPLIFETIME_WITH_PARAMS_FAST(ADCPlayerCharacter, bAiming, Params);

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

void ADCPlayerCharacter::BlendCamera()
{
}

void ADCPlayerCharacter::SetIsHiding(const bool bPlayerHiding)
{
	bIsHiding = bPlayerHiding;
}

void ADCPlayerCharacter::StartChase() const
{
	if (!IsLocallyControlled())
	{
		return;
	}

	MusicManagerComponent->StartChase();
}

ADCInteractableObject* ADCPlayerCharacter::GetLastInteractedObject() const
{
	return LastInteractedObject.Get();
}

UDCFlashlightComponent* ADCPlayerCharacter::GetFlashlightComponent() const
{
	return FlashlightComponent;
}

UDCPickupManagerComponent* ADCPlayerCharacter::GetPickupManagerComponent() const
{
	return PickupManagerComponent;
}

UDCInventoryComponent* ADCPlayerCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

EDCMovementState ADCPlayerCharacter::GetMovementState() const
{
	return CurrentPlayerMovementState;
}

bool ADCPlayerCharacter::IsHiding() const
{
	return bIsHiding;
}

bool ADCPlayerCharacter::IsDead() const
{
	return bIsDead;
}

UDCPlayerHUD* ADCPlayerCharacter::GetPlayerHUD() const
{
	return PlayerHUD;
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

void ADCPlayerCharacter::OnPlayerCaught(FGameplayTag CatchingCharacter)
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

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

	OwningPlayerState->SetPlayerDead(true, CatchingCharacter);
}

// Called to bind functionality to input
void ADCPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::Interact);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ADCPlayerCharacter::EndSprint);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &ADCPlayerCharacter::StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &ADCPlayerCharacter::EndAim);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &ADCPlayerCharacter::ShootGun);
		EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Completed, FlashlightComponent.Get(), &UDCFlashlightComponent::UseLight);
		EnhancedInputComponent->BindAction(HotbarAction1, ETriggerEvent::Started, this, &ADCPlayerCharacter::Hotbar1Press);
		EnhancedInputComponent->BindAction(HotbarAction2, ETriggerEvent::Started, this, &ADCPlayerCharacter::Hotbar2Press);
		EnhancedInputComponent->BindAction(HotbarAction3, ETriggerEvent::Started, this, &ADCPlayerCharacter::Hotbar3Press);
		EnhancedInputComponent->BindAction(HotbarAction4, ETriggerEvent::Started, this, &ADCPlayerCharacter::Hotbar4Press);
		EnhancedInputComponent->BindAction(UseItemAction, ETriggerEvent::Completed, InventoryComponent.Get(), &UDCInventoryComponent::PlayerPressedUse);
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

void ADCPlayerCharacter::StartAim()
{
	SpringArmComponent->SetActive(true);
	PlayerArm->SetActive(false);
	PlayerBody->SetActive(false);
	PlayerArm->SetOwnerNoSee(true);
	PlayerBody->SetOwnerNoSee(true);
	
	FVector CurrentOffset = SpringArmComponent->SocketOffset;
	FVector AddOffset = FVector(-150.0f, 50.0f, 0.0f);
	SpringArmComponent->SocketOffset = CurrentOffset + AddOffset;
	GetMesh()->SetOwnerNoSee(false);

	Handgun->SetActive(true);
	
	Server_OnStartAim();
}

void ADCPlayerCharacter::EndAim()
{
	PlayerArm->SetActive(true);
	PlayerBody->SetActive(true);
	PlayerArm->SetOwnerNoSee(false);
	PlayerBody->SetOwnerNoSee(false);
	
	FVector CurrentOffset = SpringArmComponent->SocketOffset;
	FVector AddOffset = FVector(150.0f, -50.0f, 0.0f);
	SpringArmComponent->SocketOffset = CurrentOffset + AddOffset;
	GetMesh()->SetOwnerNoSee(true);

	Handgun->SetActive(false);
	
	Server_OnEndAim();
}

void ADCPlayerCharacter::Server_OnStartAim_Implementation()
{
	Netmulticast_OnStartAim();
}

void ADCPlayerCharacter::Server_OnEndAim_Implementation()
{
	Netmulticast_OnEndAim();
}

void ADCPlayerCharacter::Netmulticast_OnStartAim_Implementation()
{
	bAiming = true;
	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerCharacter, bAiming, this);
	PlayAnimMontage(GunIdleMontage);
}

void ADCPlayerCharacter::Netmulticast_OnEndAim_Implementation()
{
	bAiming = false;
	MARK_PROPERTY_DIRTY_FROM_NAME(ADCPlayerCharacter, bAiming, this);
	StopAnimMontage(GunIdleMontage);
}

void ADCPlayerCharacter::Server_ShootGun_Implementation()
{
	ShootGun();
}

void ADCPlayerCharacter::ShootGun()
{
	if (!HasAuthority())
	{
		Server_ShootGun();
		return;
	}
	
	if (bAiming)
	{
		Handgun->Shoot();
	}
}

void ADCPlayerCharacter::Hotbar1Press()
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	InventoryComponent->SetSelectedItem(1);
}

void ADCPlayerCharacter::Hotbar2Press()
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	InventoryComponent->SetSelectedItem(2);
}

void ADCPlayerCharacter::Hotbar3Press()
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	InventoryComponent->SetSelectedItem(3);
}

void ADCPlayerCharacter::Hotbar4Press()
{
	if (!IsValid(InventoryComponent))
	{
		return;
	}

	InventoryComponent->SetSelectedItem(4);
}

void ADCPlayerCharacter::CheckForItem()
{
	if (!IsValid(PlayerHUD))
	{
		return;
	}

	ADCInteractableObject* const HitObject = Cast<ADCInteractableObject>(InteractCheck());
	if (!IsValid(HitObject))
	{
		PlayerHUD->HideActionPrompt();
		return;
	}

	PlayerHUD->DisplayActionPrompt(HitObject->GetActionText());
}

void ADCPlayerCharacter::SetPlayerInfoName()
{
	UDCUIPlayerInfoWidget* const PlayerInfoWidget = Cast<UDCUIPlayerInfoWidget>(PlayerInfoWidgetComponent->GetWidget());

	PlayerInfoWidget->SetTrackedPlayer(this, CustomPlayerName);
}

void ADCPlayerCharacter::LoadSoundFiles()
{
	FStreamableManager& BaseLoader = UAssetManager::GetStreamableManager();
	TArray<FSoftObjectPath> AudioSoftObjectPaths = {};

	for (const TSoftObjectPtr<USoundBase>& SoundBase : DefaultFootstepSounds)
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

	PlayerInfoWidget->SetTrackedPlayer(this, CustomPlayerName);
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

	if (!HasAuthority() && IsLocallyControlled())
	{
		Server_ReportNoise();
		return;
	}

	MakeNoise(PlayerLoudness, this, GetActorLocation());
}

void ADCPlayerCharacter::Client_DisableVoiceChat_Implementation()
{
	UKismetSystemLibrary::ExecuteConsoleCommand(this, "ToggleSpeaking 0", GetController<APlayerController>());
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
	EndLocation.Z -= 250.f;
	const FCollisionQueryParams TraceParams(FName(TEXT("")), true, GetOwner());
	GetWorld()->LineTraceSingleByChannel(Hit, PlayerLocation, EndLocation, ECC_Visibility, TraceParams);

	const AActor* const HitActor = Hit.GetActor();
	if (IsValid(HitActor))
	{
		check(FootstepData);

		FName ActorTag = "";
		if (HitActor->Tags.IsValidIndex(0))
		{
			ActorTag = HitActor->Tags[0];
		}

		int32 RandomIndex;
		USoundBase* SoundToPlay = nullptr;

		FDCFootstepSounds FootstepSoundsStruct = FootstepData->FootstepsMap.FindRef(ActorTag);
		if (!FootstepSoundsStruct.FootstepSounds.IsEmpty())
		{
			RandomIndex = FMath::RandRange(0, FootstepSoundsStruct.FootstepSounds.Num() - 1);

			if (FootstepSoundsStruct.FootstepSounds.IsValidIndex(RandomIndex))
			{
				SoundToPlay = FootstepSoundsStruct.FootstepSounds[RandomIndex];
			}
		}
		else
		{
			RandomIndex = FMath::RandRange(0, DefaultFootstepSounds.Num() - 1);
			if (DefaultFootstepSounds.IsValidIndex(RandomIndex))
			{
				SoundToPlay = DefaultFootstepSounds[RandomIndex].Get();
			}
		}

		if (!IsValid(SoundToPlay))
		{
			return;
		}
		
		check(FootstepAudioComponent);

		FootstepAudioComponent->SetSound(SoundToPlay);
		FootstepAudioComponent->Play();

		// This is no longer used
		//ReportNoise();
	}
}

void ADCPlayerCharacter::Interact()
{
	if (!HasAuthority())
	{
		Server_Interact();
	}

	AActor* ActorHit = InteractCheck();
	if (IsValid(ActorHit))
	{
		ADCInteractableObject* const InteractableObject = Cast<ADCInteractableObject>(ActorHit);
		if (IsValid(InteractableObject))
		{
			InteractableObject->Interact(this);
		}
	}
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

void ADCPlayerCharacter::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (!OtherActor->IsA<ADCInteractableObject>())
	{
		return;
	}

	UStaticMeshComponent* const StaticMeshComponent = OtherActor->GetComponentByClass<UStaticMeshComponent>();

	StaticMeshComponent->SetOverlayMaterial(ItemDetectionMaterial);
}

void ADCPlayerCharacter::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!IsValid(OtherActor))
	{
		return;
	}

	if (!OtherActor->IsA<ADCInteractableObject>())
	{
		return;
	}

	UStaticMeshComponent* const StaticMeshComponent = OtherActor->GetComponentByClass<UStaticMeshComponent>();

	StaticMeshComponent->SetOverlayMaterial(nullptr);
}

AActor* ADCPlayerCharacter::InteractCheck()
{
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

	return ActorHit;
}

void ADCPlayerCharacter::Server_Interact_Implementation()
{
	Interact();
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