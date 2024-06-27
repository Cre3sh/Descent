// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"


// Sets default values
ATutorialCharacter::ATutorialCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	cam->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATutorialCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATutorialCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATutorialCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("Continue", IE_Pressed, this, &ATutorialCharacter::LoadGame);
}

void ATutorialCharacter::LoadGame()
{
	UGameplayStatics::OpenLevel(this, "House");
}
