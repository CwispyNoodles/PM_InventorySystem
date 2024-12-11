// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventorySlotWidget.h"
#include "Widgets/InventoryWidget.h"
#include "DynamicInventoryWidget.generated.h"

class UDynamicInventoryScript;
class UWrapBox;
class UInventorySlotWidget;

USTRUCT(BlueprintType)
struct PM_INVENTORYSYSTEM_API FDynamicInventorySlotHandle
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FIntPoint Dimensions = FIntPoint(2);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInventorySlotHandle InventorySlotHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UDynamicInventoryScript> SlotScript;
};
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UDynamicInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()

public:
	virtual void NativePreConstruct() override;

	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual void TryRemoveItem(UItemInstance* ItemToRemove) override;

	virtual UInventoryData* CreateNewInventoryData(UObject* Payload) override;
	virtual void SetInventoryData(UInventoryData* NewInventoryData) override;

	UFUNCTION(BlueprintCallable)
	void Refresh();

	UFUNCTION(BlueprintCallable)
	void AddDynamicInventorySlotHandles(TArray<FDynamicInventorySlotHandle> InHandles);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FDynamicInventorySlotHandle> DynamicInventorySlotHandles;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UInventorySlotWidget> EntryWidgetClass;

private:

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UWrapBox> Container;
	
};
