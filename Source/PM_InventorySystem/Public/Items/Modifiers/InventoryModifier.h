// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData/DynamicInventoryInterface.h"
#include "Items/Modifiers/ItemModifier.h"
#include "Widgets/DynamicInventoryWidget.h"
#include "InventoryModifier.generated.h"

class UInventoryWidget;
class UInventoryData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryItemHandledModifierEvent, UItemInstance*, ItemInstance);

/**
 * A Modifier for items that contain their own inventories.
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UInventoryModifier : public UItemModifier, public IDynamicInventoryInterface
{
	GENERATED_BODY()

public:
	virtual void InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject = nullptr) override;

	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable)
	bool TryRemoveItem(UItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool InventoryItemAdded(UItemInstance* InItemInstance);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	bool InventoryItemRemoved(UItemInstance* InItemInstance);

	virtual void GetDynamicInventorySlotHandles(TArray<FDynamicInventorySlotHandle>& InArray) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FDynamicInventorySlotHandle> DefaultAttachmentSlots;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventoryData> InventoryData;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventorySlotData> AttachmentsSlotData;

	UPROPERTY(BlueprintAssignable)
	FInventoryItemHandledModifierEvent OnItemAdded;
	
	UPROPERTY(BlueprintAssignable)
	FInventoryItemHandledModifierEvent OnItemRemoved;
};
