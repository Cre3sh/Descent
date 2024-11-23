// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>
#include <GameplayTagContainer.h>

#include "DCInventoryComponent.generated.h"

class UDCItemsData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UDCInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDCInventoryComponent();

	// Begin UActorComponent override
	virtual void BeginPlay() override;
	// End UActorComponent override

	FGameplayTag GetItemAtIndex(int32 Index) const;

	int32 GetSelectedIndex() const;

	int32 GetItemAmount(FGameplayTag ItemTag) const;

	bool AddItem(FGameplayTag ItemTag, int32 Amount);

	UFUNCTION(Server, Reliable)
	void Server_UseItem(int32 Index);
	void UseItem(int32 Index);
	
	void SetSelectedItem(int32 ItemIndex);

	void PlayerPressedUse();

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDCItemsData> ItemsData = nullptr;

private:
	// The map representing the players inventory, with the amount of each item
	TMap<FGameplayTag, int32> ItemsMap;

	// The array representing which item goes to which index, so an item at index 0 will mean the user has to press '1' to access that item
	TArray<FGameplayTag> ItemArray;

	int32 SelectedItemIndex = 1;
};