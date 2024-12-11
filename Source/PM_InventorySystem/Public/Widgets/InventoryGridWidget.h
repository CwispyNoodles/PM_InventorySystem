// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonInventoryWidget.h"
#include "InventoryGridWidget.generated.h"

class UInventoryGridData;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PM_INVENTORYSYSTEM_API UInventoryGridWidget : public UCommonInventoryWidget
{
	GENERATED_BODY()

public:
	virtual UInventoryData* CreateNewInventoryData(UObject* Payload) override;

	virtual void SetInventoryData(UInventoryData* NewInventoryData) override;

	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual void TryRemoveItem(UItemInstance* ItemToRemove) override;
	virtual void AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) override;
	virtual void RemoveItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) override;

	// UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// UUserWidget Interface End

	UFUNCTION(BlueprintCallable)
	UInventoryGridData* GetInventoryGridData() const;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint ItemDragPos;

	UPROPERTY(BlueprintReadOnly)
	FIntPoint ItemMousePos;
};
