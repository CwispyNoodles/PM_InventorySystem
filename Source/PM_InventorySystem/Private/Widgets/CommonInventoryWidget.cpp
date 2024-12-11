// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/CommonInventoryWidget.h"

#include "InventoryDeveloperSettings.h"
#include "InventoryStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SizeBox.h"
#include "InventoryData/CommonInventoryData.h"
#include "Widgets/ItemWidget.h"

UInventoryData* UCommonInventoryWidget::CreateNewInventoryData(UObject* Payload)
{
	if (InventoryData)
	{
		return InventoryData;
	}
	UCommonInventoryData* NewInventoryData = NewObject<UCommonInventoryData>(Payload);
	SetInventoryData(NewInventoryData);
	return NewInventoryData;
}

void UCommonInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// ItemWidgetDraggedListenerHandle = MessageSubsystem.RegisterListener(TAG_Message_UI_ItemWidget_Dragged, this, &ThisClass::OnItemWidgetDragged);
	// ItemWidgetDroppedListenerHandle = MessageSubsystem.RegisterListener(TAG_Message_UI_ItemWidget_Dropped, this, &ThisClass::OnItemWidgetDropped);
}

void UCommonInventoryWidget::NativeDestruct()
{
	Super::NativeDestruct();

	// UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	// MessageSubsystem.UnregisterListener(ItemWidgetDraggedListenerHandle);
	// MessageSubsystem.UnregisterListener(ItemWidgetDroppedListenerHandle);
}

void UCommonInventoryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	bDrawDropLocation = true;
}

void UCommonInventoryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bDrawDropLocation = false;
}

int32 UCommonInventoryWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (!bDrawBorder)
	{
		return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	}
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	OnPaint(Context);

	if (const UInventoryDeveloperSettings* IDS = UInventoryStatics::GetInventoryDeveloperSettings())
	{
		const float EquipmentSlot_Rows = Dimensions.Y;
		const float EquipmentSlot_Columns = Dimensions.X;
		UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(0,0), FVector2D(0, EquipmentSlot_Rows) * IDS->TileSize, BorderColor);
		UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(0, EquipmentSlot_Rows) * IDS->TileSize, FVector2D(EquipmentSlot_Columns, EquipmentSlot_Rows) * IDS->TileSize, BorderColor);
		UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(EquipmentSlot_Columns, EquipmentSlot_Rows) * IDS->TileSize, FVector2D(EquipmentSlot_Columns, 0) * IDS->TileSize, BorderColor);
		UWidgetBlueprintLibrary::DrawLine(Context, FVector2D(EquipmentSlot_Columns, 0) * IDS->TileSize, FVector2D(0,0), BorderColor);
	}
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

void UCommonInventoryWidget::SetInventoryBounds(FVector2D InInventoryBounds) const
{
	if (InventoryBounds != nullptr)
	{
		InventoryBounds->SetHeightOverride(InInventoryBounds.Y);
		InventoryBounds->SetWidthOverride(InInventoryBounds.X);
	}
}

FVector2D UCommonInventoryWidget::GetInventoryBounds() const
{
	return FVector2D(InventoryBounds->GetWidthOverride(), InventoryBounds->GetHeightOverride());
}
