// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Menu/DCMenuCharacter.h"

#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>

#include "UI/Systems/DCUISceneManager.h"
#include "UI/Tags/DCSceneTags.h"

// Sets default values
ADCMenuCharacter::ADCMenuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADCMenuCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(this, 0);

	if (IsValid(PlayerController))
	{
		if (PlayerController->IsLocalController())
		{
			SceneManager = Cast<UDCUISceneManager>(CreateWidget(PlayerController, SceneManagerWidgetClass));

			if (!IsValid(SceneManager))
			{
				return;
			}

			SceneManager->AddToViewport();
			SceneManager->OpenScene(Tag_UI_Scene_MainMenu);
		}
	}	
}

