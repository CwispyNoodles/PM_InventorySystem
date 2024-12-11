// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryData.generated.h"

class UInventorySlotData;
struct FGameplayTag;
class UCommonInventoryData;
class UItemInstance;
class UInventoryWidget;
/**
 * 
 */
UCLASS(Blueprintable)
class PM_INVENTORYSYSTEM_API UInventoryData : public UObject
{
	GENERATED_BODY()

public:
	// Payload can be anything 
	UFUNCTION(BlueprintCallable)
	virtual void InitializeInventoryData(TSubclassOf<UInventoryWidget> InInventoryWidgetClass, UObject* InWorldContextObject, UObject* Payload = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryWidget* CreateInventoryWidgetInstance();

	UFUNCTION(BlueprintCallable)
	virtual bool TryAddItem(UItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	virtual bool TryRemoveItem(UItemInstance* ItemToRemove);

	UFUNCTION(BlueprintCallable)
	virtual bool ContainsItem(UItemInstance* InItemInstance);
	
	UFUNCTION(BlueprintCallable)
	TArray<UInventoryData*> GetChildInventoriesArray();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual void FindItemByTagWithSmallestStackSize(FGameplayTag InItemTag, UItemInstance*& InItemInstance, bool& bSuccess);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual void FindItemsByTag(FGameplayTag InItemTag, TArray<UItemInstance*>& InArray, bool bSortAscending = true);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventorySlotData* GetInventorySlotDataByTag(FGameplayTag InventorySlotTag, bool& bSuccess);

	UPROPERTY(BlueprintReadOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	

private:
	void SetNewOwner(UObject* NewOwner);

	UPROPERTY()
	TArray<UInventoryData*> ItemAddOrder;

protected:
	virtual UWorld* GetWorld() const override;

	// Each connected inventory.
	UPROPERTY()
	TMap<FString, UInventoryData*> ChildInventories;
	
	UPROPERTY()
	UObject* Owner;

	UPROPERTY()
	TArray<UInventorySlotData*> InventorySlotData;
};
