// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Objectives/DCSharedItemSubsystem.h"

void UDCSharedItemSubsystem::NetMulticast_AddItem_Implementation(FGameplayTag InTag)
{
	if (ItemsMap.Contains(InTag))
	{
		int32* ItemAmount = ItemsMap.Find(InTag);

		*ItemAmount = *ItemAmount + 1;
	}
	else
	{
		ItemsMap.Add(InTag, 1);
	}
}

bool UDCSharedItemSubsystem::ConsumeItem(FGameplayTag InTag)
{
	int32* ItemAmount = ItemsMap.Find(InTag);

	if (ItemAmount == nullptr)
	{
		return false;
	}

	if (*ItemAmount <= 0)
	{
		return false;
	}

	*ItemAmount = *ItemAmount - 1;

	return true;
}

TMap<FGameplayTag, int32> UDCSharedItemSubsystem::GetItemMap()
{
	return ItemsMap;
}
