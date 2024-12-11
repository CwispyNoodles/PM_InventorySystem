// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/CommonInventoryWidget.h"
#include "InventorySlotWidget.generated.h"

class UBorder;
class UTextBlock;
class UInventorySlotData;

UENUM(BlueprintType)
namespace EAttachmentMode
{
	enum AttachmentType
	{
		NONE	UMETA(DisplayName = "None"),
		SOCKET	UMETA(DisplayName = "Socket"),
		OVERRIDE	UMETA(DisplayName = "Override"),
	};
}

USTRUCT(BlueprintType)
struct PM_INVENTORYSYSTEM_API FInventorySlotHandle
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EquipmentSlotTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer ItemWhitelist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bRotateOnDrop = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Render Transform")
	bool bIsSlotNameVisible = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Render Transform", meta=(EditCondition="bIsSlotNameVisible==true"))
	FText SlotNameText = FText::FromString("PLACEHOLDER");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Render Transform", meta=(EditCondition="bIsSlotNameVisible==true"))
	FWidgetTransform SlotNameRenderTransform;

	// Attaches to component on owner that contains this tag. Defaults to root component.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Attachments")
	FName AttachmentComponentTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Attachments")
	TEnumAsByte<EAttachmentMode::AttachmentType> AttachmentType = EAttachmentMode::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Settings|Attachments", meta=(EditCondition="AttachmentType==EAttachmentMode::SOCKET", EditConditionHides))
	FName SocketName;
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class PM_INVENTORYSYSTEM_API UInventorySlotWidget : public UCommonInventoryWidget
{
	GENERATED_BODY()
	public:
	virtual UInventoryData* CreateNewInventoryData(UObject* Payload) override;

	// UInventoryWidget Interface
	virtual void SetInventoryData(UInventoryData* NewInventoryData) override;
	virtual void AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos) override;
	virtual bool TryAddItem(UItemInstance* ItemToAdd) override;
	virtual void TryRemoveItem(UItemInstance* ItemToRemove) override;
	// virtual bool ContainsItem(UItemInstance* ItemToCheck) override;
	virtual void OnItemWidgetDragged(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage) override;
	virtual void OnItemWidgetDropped(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage) override;
	// UInventoryWidget Interface End

	// UUserWidget Interface
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// UUserWidget Interface End

	UFUNCTION(BlueprintCallable)
	void SetDimensions(FIntPoint NewDimensions) { Dimensions = NewDimensions; }

	UFUNCTION(BlueprintCallable)
	void SetInventorySlotHandle(FInventorySlotHandle InInventorySlotHandle) { InventorySlotHandle = InInventorySlotHandle; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventorySlotData* GetInventorySlotData() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	FInventorySlotHandle InventorySlotHandle;

private:
	UFUNCTION(BlueprintCallable)
	void HandleIsDraggingItemWidget();
protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess, BindWidget))
	TObjectPtr<UBorder> DropIndicator;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock> SlotName;
};
