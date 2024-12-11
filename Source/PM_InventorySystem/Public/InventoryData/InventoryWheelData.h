// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData/CommonInventoryData.h"
#include "InventoryWheelData.generated.h"

class UInventoryWheelWidget;
/**
 * Unlike Inventory Slots and Grids, Belts are built to hold an unlimited amount of items
 * in a belt like fashion, similar to a circular linked list.
 * The Inventory Belt data should be used for things like item wheels.
 *
 * They are also added sequentially. 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UInventoryWheelData : public UCommonInventoryData
{
	GENERATED_BODY()

	friend UInventoryWheelWidget;
	
	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual bool TryRemoveItem(UItemInstance* ItemToRemove) override;
	virtual bool ContainsItem(UItemInstance* ItemToCheck) override;

	virtual void FindItemsByTag(FGameplayTag InItemTag, TArray<UItemInstance*>& InArray, bool bSortAscending) override;
};
