// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Actor.h>
#include <GameplayTagContainer.h>

#include "DCInteractableObject.generated.h"

class UMaterialBillboardComponent;
struct FGameplayTag;
class ADCPlayerCharacter;

UCLASS()
class ADCInteractableObject : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ADCInteractableObject();

	// Begin AActor override
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// End AActor override

	FGameplayTag GetInteractionObjectTag() const;
	
	virtual void Interact(ADCPlayerCharacter* InteractingCharacter);

	UFUNCTION(Server, Reliable)
	void Server_SetIsBeingInteracted(const bool bIsInteracted);
	void SetIsBeingInteracted(const bool bIsInteracted);

	bool IsBeingInteracted() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTag InteractionObjectTag = FGameplayTag::EmptyTag;
	
private:
	UPROPERTY(Replicated)
	bool bIsBeingInteracted = false;
};