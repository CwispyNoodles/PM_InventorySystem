// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryData/CommonInventoryData.h"
#include "InventoryGridData.generated.h"

/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UInventoryGridData : public UCommonInventoryData
{
	GENERATED_BODY()

public:
	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual bool TryRemoveItem(UItemInstance* ItemToRemove) override;
	virtual bool ContainsItem(UItemInstance* ItemToCheck) override;

	virtual void FindItemByTagWithSmallestStackSize(FGameplayTag InItemTag, UItemInstance*& InItemInstance, bool& bSuccess) override;
	virtual void FindItemsByTag(FGameplayTag InItemTag, TArray<UItemInstance*>& InArray, bool bSortAscending) override;

	UFUNCTION(BlueprintCallable)
	FVector2D IndexToTile(int32 InIndex) const;
	
	UFUNCTION(BlueprintCallable)
	int32 TileToIndex(FIntPoint InTile) const;
	
	UFUNCTION(BlueprintCallable)
	bool IsIndexOccupied(int32 InIndex) const;

	UFUNCTION(BlueprintCallable)
	void AddItemAtIndex(UItemInstance* InItemInstance, int32 InIndex);

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable(UItemInstance* InItemInstance, int32 TopLeftIndex) const;

	void SetDimensions(FIntPoint NewDimensions);

	UFUNCTION(BlueprintCallable)
	TArray<bool> GetItemGrid() {return ItemGrid; }

	void SetItemGridNum(int32 Num) { ItemGrid.SetNum(Num, true); }

	UFUNCTION()
	TMap<UItemInstance*, FVector2D> GetItemCoords() { return ItemCoords; }

	UPROPERTY()
	TArray<bool> ItemGrid = TArray<bool>();

	UPROPERTY()
	TMap<UItemInstance*, FVector2D> ItemCoords;
private:
	FIntPoint Dimensions;
};
