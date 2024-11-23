// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Inventory/DCInventoryComponent.h"

#include "DCUsableItem.h"
#include "Data/DCItemsData.h"

UDCInventoryComponent::UDCInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDCInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

FGameplayTag UDCInventoryComponent::GetItemAtIndex(int32 Index) const
{
	if (!ItemArray.IsValidIndex(Index - 1))
	{
		return FGameplayTag::EmptyTag;
	}
	
	return ItemArray[Index - 1];
}

int32 UDCInventoryComponent::GetSelectedIndex() const
{
	return SelectedItemIndex;
}

int32 UDCInventoryComponent::GetItemAmount(FGameplayTag ItemTag) const
{
	if (ItemsMap.Contains(ItemTag))
	{
		return ItemsMap.FindRef(ItemTag);
	}

	return 0;
}

bool UDCInventoryComponent::AddItem(FGameplayTag ItemTag, int32 Amount)
{
	if (ItemsMap.Contains(ItemTag))
	{
		ItemsMap[ItemTag] += Amount;
	}
	else
	{
		ItemsMap.Add(ItemTag, Amount);
		ItemArray.Add(ItemTag);
	}

	return true;
}

void UDCInventoryComponent::Server_UseItem_Implementation(int32 Index)
{
	UseItem(Index);
}

void UDCInventoryComponent::UseItem(int32 Index)
{
	check(ItemsData);

	if (!GetOwner()->HasAuthority())
	{
		Server_UseItem(Index);
	}

	if (!ItemArray.IsValidIndex(Index - 1))
	{
		return;
	}

	const FGameplayTag SelectedItem = ItemArray[Index - 1];

	TSubclassOf<ADCUsableItem> ActorToSpawn = ItemsData->ItemMap.FindRef(SelectedItem);
	if (!IsValid(ActorToSpawn))
	{
		return;
	}

	if (GetOwner()->HasAuthority())
	{
		ADCUsableItem* const UsableItem = GetWorld()->SpawnActor<ADCUsableItem>(ActorToSpawn, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);

		UsableItem->SetOwner(GetOwner());
		UsableItem->Use();
	}

	ItemsMap[SelectedItem] -= 1;
	int32 NewAmount = ItemsMap[SelectedItem];

	if (NewAmount <= 0)
	{
		ItemArray.RemoveAt(Index - 1);
		ItemsMap.Remove(SelectedItem);
	}
}

void UDCInventoryComponent::SetSelectedItem(int32 ItemIndex)
{
	SelectedItemIndex = ItemIndex;
}

void UDCInventoryComponent::PlayerPressedUse()
{
	UseItem(SelectedItemIndex);
}
