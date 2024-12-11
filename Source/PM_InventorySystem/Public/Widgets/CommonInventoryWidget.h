// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryVerbMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Widgets/InventoryWidget.h"
#include "CommonInventoryWidget.generated.h"

class UItemWidget;
class USlateBrushAsset;
class UCanvasPanel;
class UImage;
class USizeBox;
class UCommonInventoryData;
/**
 * 
 */
UCLASS(NotBlueprintable, Abstract)
class PM_INVENTORYSYSTEM_API UCommonInventoryWidget : public UInventoryWidget
{
	GENERATED_BODY()

public:
	virtual UInventoryData* CreateNewInventoryData(UObject* Payload) override;

	UFUNCTION()
	virtual void AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) PURE_VIRTUAL("AddItemWidget");

	UFUNCTION()
	virtual void RemoveItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) PURE_VIRTUAL("RemoveItemWidget");

	// UUserWidget Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// UUserWidget Interface End

	// If the AddOrderPriority is negative, it is not considered.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 AddOrderPriority = -1;
	
	UPROPERTY()
	bool bDrawDropLocation = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory", meta=(ClampMin="1"))
	FIntPoint Dimensions = FIntPoint(5);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	FColor BorderColor = FColor::Silver;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	USlateBrushAsset* SlateBrushAsset;

private:
	UFUNCTION()
	virtual void OnItemWidgetDragged(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage) PURE_VIRTUAL("OnItemWidgetDragged");
	
	UFUNCTION()
	virtual void OnItemWidgetDropped(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage) PURE_VIRTUAL("OnItemWidgetDropped");
	
	FGameplayMessageListenerHandle ItemWidgetDraggedListenerHandle;
	FGameplayMessageListenerHandle ItemWidgetDroppedListenerHandle;

protected:
	void SetInventoryBounds(FVector2D InInventoryBounds) const;
	FVector2D GetInventoryBounds() const;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<USizeBox> InventoryBounds;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UImage> InventoryImage;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> InventoryCanvasPanel;

	UPROPERTY(EditDefaultsOnly)
	bool bDrawBorder = true;
	
	UPROPERTY()
	TMap<UItemInstance*, UUserWidget*> ItemToWidgetMap;
	
};
