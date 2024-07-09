// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Character.h>
#include <Runtime/MediaAssets/Public/MediaSoundComponent.h>

#include "DCPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDCOnPlayerDied);

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
class USceneCaptureComponent2D;
class UDCPlayerHUD;
class USpringArmComponent;
class USpotLightComponent;
class UCameraComponent;

UENUM()
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

	// Begin AActor override
	virtual void BeginPlay() override;
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

	void OnPlayerCaught();

	UMediaSoundComponent* GetMediaSoundComponent() const;

	UDCUISceneManager* GetSceneManager() const;

	ADCInteractableObject* GetLastInteractedObject() const;

	UDCPickupManagerComponent* GetPickupManagerComponent() const;
	
	FDCOnPlayerDied OnPlayerDied;

private:
	UFUNCTION(Server, Reliable)
	void Server_SetPlayerName(const FText& PlayerName);

	UFUNCTION(Server, Reliable)
	void Server_ReportNoise();

	/*Report a sound that was played in our game*/
	UFUNCTION(BlueprintCallable, Category = AI)
	void ReportNoise();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void UpdateFootsteps(const float DeltaTime);

	UFUNCTION(Server, Reliable)
	void Server_UpdateMovementState();
	void UpdateMovementState();
	
	UFUNCTION()
	void OnRep_CustomPlayerName();
	UPROPERTY(ReplicatedUsing = OnRep_CustomPlayerName)
	FText CustomPlayerName = FText::GetEmpty();

	void UpdateCamera();

	/*Check Material under the Player*/
	void FootCheck();

	UFUNCTION(Server, Reliable)
	void Server_InteractCheck();

	/*Check for player interactions*/
	void InteractCheck();

	void StartCrouch();
	void EndCrouch();

	UFUNCTION(Server, Reliable)
	void Server_StartSprint();
	void StartSprint();

	UFUNCTION(Server, Reliable)
	void Server_EndSprint();
	void EndSprint();

	void SetPlayerInfoName();

	/*Toggles the flashlight*/
	void ToggleLight() const;

	// Called at game start
	void LoadSoundFiles();

	/*Player interaction range*/
	float Reach = 200.f;

	/*Used for the parameter in the make noise function*/
	float PlayerLoudness = 0.25f;
	
	bool bSprinting;
	bool bCrouching;

	bool HasFlashlight = false;

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
	TObjectPtr<UWidgetComponent> PlayerInfoWidgetComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSoftObjectPtr<USoundBase>> FootstepSounds = {};

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> PlayerCamera = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<ACameraActor> SpectatorCameraActor = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TObjectPtr<USceneComponent> SpectatorCameraHolder = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TObjectPtr<USpringArmComponent> MinimapSpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Minimap")
	TObjectPtr<USceneCaptureComponent2D> SceneCaptureComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TObjectPtr<UDCPickupManagerComponent> PickupManagerComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UDCTerrorRadiusComponent> TerrorComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> FootstepAudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UAudioComponent> FlashlightAudioComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UMediaSoundComponent> ApparitionDeathSound = nullptr;
	
	/*This will be used to give our player the ability to emit sounds the AI can hear*/
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<UPawnNoiseEmitterComponent> PawnNoiseEmitterComp;

	// A scene manager used to manage the opening and closing of scenes
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TObjectPtr<UDCUISceneManager> SceneManager = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "SpotLight")
	TObjectPtr<USpotLightComponent> Torch = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Crouch")
	FVector CrouchEyeOffset = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float WalkSpeed = 700.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
	float RunSpeed = 1350.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Crouch")
	float CrouchSpeed = 0.0f;

	UPROPERTY()
	TObjectPtr<UDCPlayerHUD> PlayerHUD = nullptr;

	UFUNCTION()
	void OnRep_LastInteractedObject();
	UPROPERTY(ReplicatedUsing = OnRep_LastInteractedObject)
	TWeakObjectPtr<ADCInteractableObject> LastInteractedObject = nullptr;

	float DistanceTravelled = 0.0f;
	
	EDCMovementState CurrentPlayerMovementState;

	FTimerHandle WidgetRetryConstruction;
};