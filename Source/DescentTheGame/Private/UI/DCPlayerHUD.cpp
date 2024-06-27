// Fill out your copyright notice in the Description page of Project Settings.

#include "DCPlayerHUD.h"

#include <Components/Image.h>
#include <MediaPlayer.h>
#include <GameFramework/GameStateBase.h>
#include <GameFramework/PlayerState.h>
#include <Components/WidgetComponent.h>

#include "Player/DCPlayerCharacter.h"
#include "Player/DCPlayerState.h"
#include "Player/DCUIPlayerInfoWidget.h"

void UDCPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();

	check(MediaPlayer);

	// Why do we open this on ctor? Because we don't want the game to hitch when the player gets caught by the monster
	// as it tries to load the media source etc.

	MediaPlayer->OpenSource(DeathSceneMediaSource);

	ADCPlayerState* const OwningPlayerState = GetOwningPlayer()->GetPlayerState<ADCPlayerState>();
	if (!IsValid(OwningPlayerState))
	{
		return;
	}

	OwningPlayerState->OnNewPlayerStateJoined.AddDynamic(this, &UDCPlayerHUD::OnNewPlayerJoined);

	// Create PlayerInfoWidgets
	const UWorld* const World = GetWorld();
	
	check(World);

	AGameStateBase* GameStateBase = World->GetGameState();
	
	check(GameStateBase);

	TArray<APlayerState*> PlayerArray = GameStateBase->PlayerArray;

	for (APlayerState* const PlayerState : PlayerArray)
	{
		SetupPlayerInfoWidgetForPlayer(PlayerState);
	}
}

void UDCPlayerHUD::NativeDestruct()
{
	Super::NativeDestruct();

	APlayerController* const PlayerController = GetOwningPlayer();
	if (!IsValid(PlayerController))
	{
		return;
	}

	ADCPlayerState* const OwningPlayerState = PlayerController->GetPlayerState<ADCPlayerState>();
	if (!IsValid(OwningPlayerState))
	{
		return;
	}

	OwningPlayerState->OnNewPlayerStateJoined.RemoveAll(this);
}

void UDCPlayerHUD::OnPlayerDied()
{
	check(DeathScreenVideoImage);
	check(MediaPlayer);

	DeathScreenVideoImage->SetVisibility(ESlateVisibility::Visible);

	const ADCPlayerCharacter* const PlayerCharacter = GetOwningPlayerPawn<ADCPlayerCharacter>();

	check(PlayerCharacter);

	UMediaSoundComponent* const MediaSoundComponent = PlayerCharacter->GetMediaSoundComponent();

	check(MediaSoundComponent);

	if (!MediaPlayer->IsReady())
	{
		UE_LOG(LogTemp, Error, TEXT("Media player wasn't ready for death scene!"));
		return;
	}
	
	MediaPlayer->Play();
}

void UDCPlayerHUD::OnNewPlayerJoined(APlayerState* PlayerState)
{
	if (!GetOwningPlayer()->IsLocalController())
	{
		return;
	}

	SetupPlayerInfoWidgetForPlayer(PlayerState);
}

void UDCPlayerHUD::SetupPlayerInfoWidgetForPlayer(APlayerState* PlayerState)
{
	if (!IsValid(PlayerState) || PlayerState == GetOwningPlayer()->PlayerState)
	{
		return;
	}

	APawn* const Pawn = PlayerState->GetPawn();
	
	if (!IsValid(Pawn))
	{
		return;
	}

	UWidgetComponent* const WidgetComponent = Cast<UWidgetComponent>(Pawn->AddComponentByClass(UWidgetComponent::StaticClass(), false, Pawn->GetActorTransform(), false));

	WidgetComponent->AttachToComponent(Pawn->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	WidgetComponent->AddRelativeLocation(FVector(0.0f, 0.0f, 100.0f));

	WidgetComponent->SetWidgetClass(PlayerInfoWidgetClass);

	WidgetComponent->SetDrawSize(FVector2D(165.0f, 165.0f));

	UDCUIPlayerInfoWidget* const PlayerInfoWidget = Cast<UDCUIPlayerInfoWidget>(WidgetComponent->GetWidget());
	if (!IsValid(PlayerInfoWidget))
	{
		return;
	}

	PlayerInfoWidget->SetTrackedPlayer(PlayerState);
}
