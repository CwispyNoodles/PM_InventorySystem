// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryData/InventorySlotData.h"

#include "Items/ItemActor.h"
#include "Items/ItemDefinition.h"
#include "Items/ItemInstance.h"

bool UInventorySlotData::TryAddItem(UItemInstance* ItemToAdd)
{
	if (ItemToAdd != nullptr && IsRoomAvailable() && IsItemWhitelisted(ItemToAdd))
	{
		Items.Add(ItemToAdd);
		ItemToAdd->SetOwner(Owner);
		OnItemAddedDelegate.Broadcast(ItemToAdd, FVector2D(0));
		// if (AttachmentComponentTag != FName(NAME_None) && AttachmentType != EAttachmentMode::NONE)
		// {
		// 	if (AActor* OwnerActor = Cast<AActor>(Owner))
		// 	{
		// 		if (AttachmentType == EAttachmentMode::SOCKET)
		// 		{
		// 			TArray<UActorComponent*> Components = OwnerActor->GetComponentsByTag(USceneComponent::StaticClass(), AttachmentComponentTag);
		// 			for (UActorComponent* Component : Components)
		// 			{
		// 				if (USceneComponent* SceneComponent = Cast<USceneComponent>(Component))
		// 				{
		// 					EquippedItemActor = ItemToAdd->SpawnItemActor();
		// 					EquippedItemActor->AttachToComponent(SceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		// 				}
		// 			}
		// 		}
		// 		else if (AttachmentType == EAttachmentMode::OVERRIDE)
		// 		{
		// 			
		// 		}
		// 	}
		// }
		return true;
	}
	return false;
}

bool UInventorySlotData::TryRemoveItem(UItemInstance* ItemToRemove)
{
	if (ContainsItem(ItemToRemove))
	{
		Items.Empty();
		// if (IsValid(EquippedItemActor.Get()))
		// {
		// 	EquippedItemActor.Get()->Destroy();
		// 	EquippedItemActor = nullptr;
		// }
		return true;
	}
	return false;
}

bool UInventorySlotData::ContainsItem(UItemInstance* ItemToCheck)
{
	return Items.Contains(ItemToCheck);
}

void UInventorySlotData::InitializeInventorySlotData(UInventorySlotWidget* InInventorySlotWidget)
{
	check(InInventorySlotWidget);

	InventorySlotHandle = InInventorySlotWidget->InventorySlotHandle;
	SetDimensions(InInventorySlotWidget->Dimensions);
}

void UInventorySlotData::FindItemByTagWithSmallestStackSize(FGameplayTag InItemTag, UItemInstance*& InItemInstance,
	bool& bSuccess)
{
	UItemInstance* SlotItem = GetSlotItem();
	if (IsValid(SlotItem) && SlotItem->GetItemDefinition()->ItemTag.MatchesTag(InItemTag))
	{
		if (!IsValid(InItemInstance))
		{
			InItemInstance = SlotItem;
		}
		else
		{
			if (InItemInstance->GetCurrentStack() >= SlotItem->GetCurrentStack())
			{
				InItemInstance = SlotItem;
			}
		}
	}
}

void UInventorySlotData::SetDimensions(FIntPoint NewDimensions)
{
	Dimensions = NewDimensions;
}

bool UInventorySlotData::IsItemWhitelisted(UItemInstance* ItemToCheck) const
{
	if (!InventorySlotHandle.ItemWhitelist.IsEmpty())
	{
		return ItemToCheck->GetItemDefinition()->ItemTag.MatchesAny(InventorySlotHandle.ItemWhitelist);
	}
	return true;
}

bool UInventorySlotData::IsRoomAvailable() const
{
	return Items.Num() != 1;
}

UItemInstance* UInventorySlotData::GetSlotItem()
{
	if (Items.IsValidIndex(0))
	{
		return Items[0];
	}
	return nullptr;
}
