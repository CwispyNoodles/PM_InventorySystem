// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Modifiers/InventoryModifier.h"

#include "InventoryData/InventoryData.h"

void UInventoryModifier::InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject)
{
	Super::InitializeItemModifier(InItemInstance, InWorldContextObject);

	InventoryData = NewObject<UInventoryData>(this);
	InventoryData->InitializeInventoryData(InventoryWidgetClass, InWorldContextObject, this);
}

bool UInventoryModifier::TryAddItem(UItemInstance* InItemInstance)
{
	if (InventoryData->TryAddItem(InItemInstance))
	{
		OnItemAdded.Broadcast(InItemInstance);
		InventoryItemAdded(InItemInstance);
		return true;
	}
	return false;
}

bool UInventoryModifier::TryRemoveItem(UItemInstance* InItemInstance)
{
	if (InventoryData->TryRemoveItem(InItemInstance))
	{
		OnItemRemoved.Broadcast(InItemInstance);
		InventoryItemRemoved(InItemInstance);
		return true;
	}
	return false;
}

void UInventoryModifier::GetDynamicInventorySlotHandles(TArray<FDynamicInventorySlotHandle>& InArray)
{
	InArray.Append(DefaultAttachmentSlots);
}
