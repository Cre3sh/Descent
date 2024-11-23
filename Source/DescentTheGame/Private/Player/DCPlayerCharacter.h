// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Character.h>

#include "DCPlayerCharacter.generated.h"

class ADCHandgun;
class UDCInventoryComponent;
class UDCMusicManagerComponent;
class UDCFootstepData;
class UDCFlashlightComponent;
class UBoxComponent;
class UDCTerrorRadiusComponent;
class UDCPickupManagerComponent;
struct FGameplayTag;
class UWidgetComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class ADCInteractableObject;
class UDCUISceneManager;
class UDCSceneData;
class UDCPlayerHUD;
class USpringArmComponent;
class UCameraComponent;

UENUM(BlueprintType)
enum EDCMovementState
{
	Idle,
	Crouching,
	Walking,
	Running
};

UCLASS()
class DESCENTTHEGAME_API ADCPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ADCPlayerCharacter();

	// Begin APawn override
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	// End APawn override
	
	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor override

	ACameraActor* GetSpectatorCamera();

	void StartedInteractingWithObject(const FGameplayTag& GameplayTag) const;

	// Currently interactions only support one at a time, so we don't need to pass in a tag
	// BUT we might want to change this later so this is probably going to be temporary
	void StoppedInteractingWithObject();

	void SetLastInteractedObject(ADCInteractableObject* InteractableObject);

	void BlendCamera();

	void SetIsHiding(const bool bPlayerHiding);

	void StartChase() const;

	void OnPlayerCaught(FGameplayTag CatchingCharacter);

	UDCPlayerHUD* GetPlayerHUD() const;
	
	UDCUISceneManager* GetSceneManager() const;

	ADCInteractableObject* GetLastInteractedObject() const;

	UDCFlashlightComponent* GetFlashlightComponent() const;

	UDCPickupManagerComponent* GetPickupManagerComponent() const;

	UDCInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable)
	EDCMovementState GetMovementState() const;

	UFUNCTION(Client, Reliable)
	void Client_DisableVoiceChat();

	UFUNCTION()
	void OnRep_CustomPlayerName();
	UPROPERTY(ReplicatedUsing = OnRep_CustomPlayerName)
	FText CustomPlayerName = FText::GetEmpty();

	bool IsHiding() const;

	bool IsDead() const;

private:
	UFUNCTION(Server, Reliable)
	void Server_SetPlayerName(const FText& PlayerName);

	UFUNCTION(Server, Reliable)
	void Server_ReportNoise();

	/*Report a sound that was played in our game*/
	UFUNCTION(BlueprintCallable, Category = AI)
	void ReportNoise();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetupPlayerHUD();

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(Server, Reliable)
	void Server_UpdateMovementState();
	void UpdateMovementState();

	void UpdateCamera();

	/*Check Material under the Player*/
	UFUNCTION(BlueprintCallable)
	void FootCheck();

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Returns the first actor that meets the interaction criteria
	AActor* InteractCheck();

	UFUNCTION(Server, Reliable)
	void Server_Interact();
	void Interact();

	void StartCrouch();
	void EndCrouch();

	UFUNCTION(Server, Reliable)
	void Server_StartSprint();
	void StartSprint();

	UFUNCTION(Server, Reliable)
	void Server_EndSprint();
	void EndSprint();

	void StartAim();
	void EndAim();

	UFUNCTION(Server, Reliable)
	void Server_OnStartAim();
	UFUNCTION(Server, Reliable)
	void Server_OnEndAim();

	UFUNCTION(NetMulticast, Reliable)
	void Netmulticast_OnStartAim();
	UFUNCTION(NetMulticast, Reliable)
	void Netmulticast_OnEndAim();

	UFUNCTION(Server, Reliable)
	void Server_ShootGun();
	void ShootGun();

	void Hotbar1Press();
	void Hotbar2Press();
	void Hotbar3Press();
	void Hotbar4Press();

	// Called every frame to see if we have an item that
	// meets interaction criteria for us to display an action prompt
	void CheckForItem();

	void SetPlayerInfoName();

	// Called at game start
	void LoadSoundFiles();

	/*Player interaction range*/
	float Reach = 200.f;

	/*Used for the parameter in the make noise function*/
	float PlayerLoudness = 0.25f;
	
	bool bSprinting;
	bool bCrouching;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> PlayerHUDClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> SceneManagerClass = nullptr;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<EDCMovementState>, TSubclassOf<UCameraShakeBase>> CameraShakes;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InteractAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> AimAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> FlashlightAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> HotbarAction1 = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> HotbarAction2 = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> HotbarAction3 = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> HotbarAction4 = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> UseItemAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> ShootAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UWidgetComponent> PlayerInfoWidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<USoundBase>> DefaultFootstepSounds = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> PlayerArm = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TObjectPtr<USkeletalMeshComponent> PlayerBody = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(AllowPrivateAccess), Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<ACameraActor> SpectatorCameraActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USceneComponent> SpectatorCameraHolder = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArmComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UDCInventoryComponent> InventoryComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TObjectPtr<UDCPickupManagerComponent> PickupManagerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UDCMusicManagerComponent> MusicManagerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> FootstepAudioComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Flashlight")
	TObjectPtr<USpringArmComponent> FlashlightSpringArmComponent = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category = "Flashlight")
	TObjectPtr<UDCFlashlightComponent> FlashlightComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Detection")
	TObjectPtr<UBoxComponent> DetectionBox = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Detection")
	TObjectPtr<UMaterial> ItemDetectionMaterial = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ADCHandgun> HandgunClass = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> GunIdleMontage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ADCHandgun> Handgun = nullptr;

	// A scene manager used to manage the opening and closing of scenes
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UDCUISceneManager> SceneManager = nullptr;

	// The data asset containing footstep sounds for each kind of surface
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UDCFootstepData> FootstepData = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Crouch")
	FVector CrouchEyeOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float WalkSpeed = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float RunSpeed = 1350.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Crouch")
	float CrouchSpeed = 0.0f;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	bool bAiming = false;

	UPROPERTY()
	TObjectPtr<UDCPlayerHUD> PlayerHUD = nullptr;

	UFUNCTION()
	void OnRep_LastInteractedObject();
	UPROPERTY(ReplicatedUsing = OnRep_LastInteractedObject)
	TWeakObjectPtr<ADCInteractableObject> LastInteractedObject = nullptr;
	
	EDCMovementState CurrentPlayerMovementState;

	FTimerHandle WidgetRetryConstruction;

	bool bNeedsHUDSetup = true;

	bool bIsHiding = false;

	bool bIsDead = false;
};