// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryData/InventoryGridData.h"

#include "Items/ItemDefinition.h"
#include "Items/ItemInstance.h"

bool UInventoryGridData::TryAddItem(UItemInstance* ItemToAdd)
{
	for (int i = 0; i < GetItemGrid().Num(); i++)
	{
		if (IsRoomAvailable(ItemToAdd, i))
		{
			AddItemAtIndex(ItemToAdd, i);
			return true;
		}
	}
	return false;
}

bool UInventoryGridData::TryRemoveItem(UItemInstance* ItemToRemove)
{
	if (ContainsItem(ItemToRemove))
	{
		const FVector2D GridTile = ItemCoords.FindRef(ItemToRemove);
		const FIntPoint ItemDimensions = ItemToRemove->GetDimensions();
		for (int32 i = GridTile.X; i < ItemDimensions.X + GridTile.X; i++)
		{
			for (int32 j = GridTile.Y; j < ItemDimensions.Y + GridTile.Y; j++)
			{
				const FIntPoint Tile = FIntPoint(i, j);
				ItemGrid[TileToIndex(Tile)] = false;
			}
		}
		Items.Remove(ItemToRemove);
		ItemCoords.Remove(ItemToRemove);
		OnItemRemovedDelegate.Broadcast(ItemToRemove, FVector2D());
		return true;
	}
	return false;
}

bool UInventoryGridData::ContainsItem(UItemInstance* ItemToCheck)
{
	return Items.Contains(ItemToCheck);
}

void UInventoryGridData::FindItemByTagWithSmallestStackSize(FGameplayTag InItemTag, UItemInstance*& InItemInstance,
	bool& bSuccess)
{
	for (UItemInstance* ItemInstance : Items)
	{
		if (ItemInstance->GetItemDefinition()->ItemTag.MatchesTag(InItemTag))
		{
			if (!IsValid(InItemInstance))
			{
				InItemInstance = ItemInstance;
			}
			else
			{
				if (InItemInstance->GetCurrentStack() >= ItemInstance->GetCurrentStack())
				{
					InItemInstance = ItemInstance;
				}
			}
		}
	}
}

void UInventoryGridData::FindItemsByTag(FGameplayTag InItemTag, TArray<UItemInstance*>& InArray, bool bSortAscending)
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

FVector2D UInventoryGridData::IndexToTile(int32 InIndex) const
{
	return FVector2D(InIndex % Dimensions.X, InIndex / Dimensions.X);
}

int32 UInventoryGridData::TileToIndex(FIntPoint InTile) const
{
	return InTile.Y * Dimensions.X + InTile.X;
}

bool UInventoryGridData::IsIndexOccupied(int32 InIndex) const
{
	if (ItemGrid.IsValidIndex(InIndex))
	{
		return ItemGrid[InIndex];
	}
	return false;
}

void UInventoryGridData::AddItemAtIndex(UItemInstance* InItemInstance, int32 InIndex)
{
	Items.Add(InItemInstance);
	const FVector2D GridTile = IndexToTile(InIndex);
	const FIntPoint ItemDimensions = InItemInstance->GetDimensions();
	for (int32 i = GridTile.X; i < ItemDimensions.X + GridTile.X; i++)
	{
		for (int32 j = GridTile.Y; j < ItemDimensions.Y + GridTile.Y; j++)
		{
			const FIntPoint Tile = FIntPoint(i, j);
			ItemGrid[TileToIndex(Tile)] = true;
		}
	}
	ItemCoords.Add(InItemInstance, GridTile);
	OnItemAddedDelegate.Broadcast(InItemInstance, GridTile);
}

bool UInventoryGridData::IsRoomAvailable(UItemInstance* InItemInstance, int32 TopLeftIndex) const
{
	const FVector2D GridTile = IndexToTile(TopLeftIndex);
	
	const FIntPoint ItemDimensions = InItemInstance->GetDimensions();
	for (int i = GridTile.X; i < ItemDimensions.X + GridTile.X; i++)
	{
		for (int j = GridTile.Y; j < ItemDimensions.Y + GridTile.Y; j++)
		{
			const FIntPoint Tile = FIntPoint(i, j);
			if (Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Dimensions.X && Tile.Y < Dimensions.Y)
			{
				if (IsIndexOccupied(TileToIndex(Tile)))
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void UInventoryGridData::SetDimensions(FIntPoint NewDimensions)
{
	Dimensions = NewDimensions;
}
