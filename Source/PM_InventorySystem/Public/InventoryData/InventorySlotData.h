// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "InventoryData/CommonInventoryData.h"
#include "Widgets/InventorySlotWidget.h"
#include "InventorySlotData.generated.h"

class AItemActor;
class UInventorySlotWidget;


UCLASS()
class PM_INVENTORYSYSTEM_API UInventorySlotData : public UCommonInventoryData
{
	GENERATED_BODY()

public:
	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual bool TryRemoveItem(UItemInstance* ItemToRemove) override;
	virtual bool ContainsItem(UItemInstance* ItemToCheck) override;
	void InitializeInventorySlotData(UInventorySlotWidget* InInventorySlotWidget);

	virtual void FindItemByTagWithSmallestStackSize(FGameplayTag InItemTag, UItemInstance*& InItemInstance, bool& bSuccess) override;

	void SetDimensions(FIntPoint NewDimensions);

	UFUNCTION(BlueprintCallable)
	bool IsItemWhitelisted(UItemInstance* ItemToCheck) const;

	UFUNCTION(BlueprintCallable)
	bool IsRoomAvailable() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)	
	UItemInstance* GetSlotItem();

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AItemActor> EquippedItemActor;

	FIntPoint Dimensions;

	UPROPERTY(BlueprintReadOnly)
	FInventorySlotHandle InventorySlotHandle;
};
