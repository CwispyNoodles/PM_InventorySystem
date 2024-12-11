// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventorySlotWidget.h"

#include "InventoryDeveloperSettings.h"
#include "InventoryStatics.h"
#include "InventorySubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "InventoryData/InventorySlotData.h"
#include "Items/ItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/ItemWidget.h"

UInventoryData* UInventorySlotWidget::CreateNewInventoryData(UObject* Payload)
{
	if (InventoryData)
	{
		return InventoryData;
	}
	UInventorySlotData* NewInventorySlotData = NewObject<UInventorySlotData>();
	NewInventorySlotData->InitializeInventorySlotData(this);
	SetInventoryData(NewInventorySlotData);
	return NewInventorySlotData;
}

void UInventorySlotWidget::SetInventoryData(UInventoryData* NewInventoryData)
{
	Super::SetInventoryData(NewInventoryData);

	if (UInventorySlotData* InventorySlotData = Cast<UInventorySlotData>(NewInventoryData))
	{
		InventorySlotHandle = InventorySlotData->InventorySlotHandle;
		if (const UInventoryDeveloperSettings* InventorySettings = UInventoryStatics::GetInventoryDeveloperSettings())
		{
			Dimensions = InventorySlotData->Dimensions;
			SetInventoryBounds(UKismetMathLibrary::Conv_IntPointToVector2D(Dimensions) * InventorySettings->TileSize);
		}
		if (UItemInstance* Item = InventorySlotData->GetSlotItem())
		{
			AddItemWidget(Item, FVector2D(0));
		}
	}
}

void UInventorySlotWidget::AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos)
{
	if (const UInventoryDeveloperSettings* IDS = UInventoryStatics::GetInventoryDeveloperSettings())
	{
		TSubclassOf<UItemWidget> ItemWidgetClass = IDS->DefaultItemWidgetClass;
		check(ItemWidgetClass)
		UItemWidget* ItemWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
		ItemWidget->InitializeItemWidget(NewItemInstance);
		if (InventorySlotHandle.bRotateOnDrop && !NewItemInstance->IsRotated())
		{
			NewItemInstance->Rotate();
		}
		UCanvasPanelSlot* CPS = InventoryCanvasPanel->AddChildToCanvas(ItemWidget);
		CPS->SetSize(UKismetMathLibrary::Conv_IntPointToVector2D(Dimensions) * IDS->TileSize);
		ItemWidget->OnItemRemoved.AddDynamic(this, &ThisClass::TryRemoveItem);
	}
}

bool UInventorySlotWidget::TryAddItem(UItemInstance* ItemToAdd)
{
	return GetInventorySlotData()->TryAddItem(ItemToAdd);
}

void UInventorySlotWidget::TryRemoveItem(UItemInstance* ItemToRemove)
{
	GetInventorySlotData()->TryRemoveItem(ItemToRemove);
}

void UInventorySlotWidget::OnItemWidgetDragged(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage)
{
	HandleIsDraggingItemWidget();
}

void UInventorySlotWidget::OnItemWidgetDropped(FGameplayTag InChannel, const FInventoryVerbMessage& InMessage)
{
	HandleIsDraggingItemWidget();
}

void UInventorySlotWidget::NativePreConstruct()
{
	if (const UInventoryDeveloperSettings* InventorySettings = UInventoryStatics::GetInventoryDeveloperSettings())
	{
		SetInventoryBounds(UKismetMathLibrary::Conv_IntPointToVector2D(Dimensions) * InventorySettings->TileSize);
	}

	InventorySlotHandle.bIsSlotNameVisible ? SlotName->SetVisibility(ESlateVisibility::HitTestInvisible) : SlotName->SetVisibility(ESlateVisibility::Hidden);
	if (InventorySlotHandle.bIsSlotNameVisible)
	{
		SlotName->SetRenderTransform(InventorySlotHandle.SlotNameRenderTransform);
		SlotName->SetText(InventorySlotHandle.SlotNameText);
	}
}

FReply UInventorySlotWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float WheelDelta = InMouseEvent.GetWheelDelta();
	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	if (InventorySubsystem->IsDraggingItem() && WheelDelta <= -1)
	{
		UItemInstance* DraggedItem = InventorySubsystem->GetDraggedItem();
		UInventorySlotData* InventorySlotData = GetInventorySlotData();
		if (InventorySlotData->IsRoomAvailable() && InventorySlotData->IsItemWhitelisted(DraggedItem))
		{
			if (DraggedItem->GetCurrentStack() == 1)
			{
				if (InventorySlotData->TryAddItem(DraggedItem))
				{
					InventorySubsystem->ClearItemDragWidget();
				}
			}
			else
			{
				UItemInstance* NewItem = DuplicateObject(DraggedItem, nullptr);
				NewItem->SetStack(1);
				int32 MaxDif;
				DraggedItem->AddStack(-1, MaxDif);
				InventorySlotData->TryAddItem(NewItem);
			}
		}
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Single Drop"));
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply =  Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	if (InventorySubsystem->IsDraggingItem() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UItemInstance* DraggedItem = InventorySubsystem->GetDraggedItem();
		UInventorySlotData* InventorySlotData = GetInventorySlotData();
		if (InventorySlotData->IsRoomAvailable() && InventorySlotData->IsItemWhitelisted(DraggedItem))
		{
			if (InventorySlotData->TryAddItem(DraggedItem))
			{
				InventorySubsystem->ClearItemDragWidget();
			}
		}
	}
	return Reply;
}

int32 UInventorySlotWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	OnPaint(Context);
	
	if (GetWorld()->IsGameWorld())
	{
		UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
		const UInventorySlotData* InventorySlotData = GetInventorySlotData();
		if (bDrawDropLocation && InventorySubsystem->IsDraggingItem() && InventorySlotData->IsRoomAvailable())
		{
			if(const UInventoryDeveloperSettings* InventorySettings = GetDefault<UInventoryDeveloperSettings>())
			{
				const FColor Tint = InventorySlotData->IsItemWhitelisted(InventorySubsystem->GetDraggedItem()) ? InventorySettings->ValidTileColor : InventorySettings->InvalidTileColor;
				UWidgetBlueprintLibrary::DrawBox(Context, FVector2D(0.0f), GetInventoryBounds(), SlateBrushAsset, Tint);
			}
		}
	}
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
							  bParentEnabled);
}

UInventorySlotData* UInventorySlotWidget::GetInventorySlotData() const
{
	return Cast<UInventorySlotData>(InventoryData);
}

void UInventorySlotWidget::HandleIsDraggingItemWidget()
{
	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	UInventorySlotData* SlotData = GetInventorySlotData();
	if (InventorySubsystem->IsDraggingItem() && SlotData->IsRoomAvailable() && SlotData->IsItemWhitelisted(InventorySubsystem->GetDraggedItem()))
	{
		DropIndicator->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	DropIndicator->SetVisibility(ESlateVisibility::Hidden);
}
