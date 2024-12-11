// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryData/InventoryWheelData.h"

#include "GameplayTagContainer.h"
#include "Items/ItemDefinition.h"
#include "Items/ItemInstance.h"

bool UInventoryWheelData::TryAddItem(UItemInstance* ItemToAdd)
{
	if (IsValid(ItemToAdd))
	{
		Items.AddUnique(ItemToAdd);
		OnItemAddedDelegate.Broadcast(ItemToAdd, FVector2D(0.0f, Items.Num()-1));
		return true;
	}
	return false;
}

bool UInventoryWheelData::TryRemoveItem(UItemInstance* ItemToRemove)
{
	if (ContainsItem(ItemToRemove))
	{
		Items.Remove(ItemToRemove);
		return true;
	}
	return false;
}

bool UInventoryWheelData::ContainsItem(UItemInstance* ItemToCheck)
{
	return Items.Contains(ItemToCheck);
}

void UInventoryWheelData::FindItemsByTag(FGameplayTag InItemTag, TArray<UItemInstance*>& InArray, bool bSortAscending)
{
	for (UItemInstance* ItemInstance : Items)
	{
		if (ItemInstance->GetItemDefinition()->ItemTag.MatchesTag(InItemTag))
		{
			if (InArray.IsEmpty())
			{
				InArray.Add(ItemInstance);
				continue;
			}
			for (int i = 0; i < InArray.Num(); i++)
			{
				if (InArray[i]->GetCurrentStack() >= ItemInstance->GetCurrentStack())
				{
					InArray.Insert(ItemInstance, i);
					break;
				}
			}
			if (!InArray.Contains(ItemInstance))
			{
				InArray.Add(ItemInstance);
			}
		}
	}
}
