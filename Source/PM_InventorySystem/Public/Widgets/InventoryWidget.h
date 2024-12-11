// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UItemInstance;
class UCommonInventoryWidget;
class UInventoryData;
/**
 * 
 */
UCLASS(Blueprintable)
class PM_INVENTORYSYSTEM_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual UInventoryData* CreateNewInventoryData(UObject* Payload = nullptr);
	
	UFUNCTION(BlueprintCallable)
	virtual void SetInventoryData(UInventoryData* NewInventoryData);

	// Can be used to run code prior Inventory data initialization.
	// UFUNCTION(BlueprintCallable)
	// virtual void InitializeInventoryWidget(UInventoryData* ParentInventoryData) PURE_VIRTUAL("InitializeInventoryWidget");
	
	// The order in which we would like to add items to each slot or grid in this inventory.
	// These slots and grids should self report on construction.
	TArray<UCommonInventoryWidget*> GetItemAddOrder() const;

	UFUNCTION(BlueprintCallable)
	UInventoryData* GetInventoryData() { return InventoryData; }

	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemInstance* ItemToAdd);

	UFUNCTION(BlueprintCallable)
	virtual void TryRemoveItem(UItemInstance* ItemToRemove) PURE_VIRTUAL("TryRemoveItem");

	UFUNCTION(BlueprintCallable)
	virtual bool ContainsItem(UItemInstance* InItemInstance);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TObjectPtr<UInventoryData> InventoryData;
};
