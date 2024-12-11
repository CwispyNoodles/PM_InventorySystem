// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryGridWidget.h"

#include "InventoryDeveloperSettings.h"
#include "InventoryStatics.h"
#include "InventorySubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "InventoryData/InventoryGridData.h"
#include "Items/ItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Widgets/ItemWidget.h"

UInventoryData* UInventoryGridWidget::CreateNewInventoryData(UObject* Payload)
{
	if (InventoryData)
	{
		return InventoryData;
	}
	UInventoryGridData* NewInventoryGridData = NewObject<UInventoryGridData>();
	SetInventoryData(NewInventoryGridData);
	NewInventoryGridData->SetItemGridNum(Dimensions.X * Dimensions.Y);
	NewInventoryGridData->SetDimensions(Dimensions);
	return NewInventoryGridData;
}

void UInventoryGridWidget::SetInventoryData(UInventoryData* NewInventoryData)
{
	Super::SetInventoryData(NewInventoryData);

	if (UInventoryGridData* InventoryGridData = Cast<UInventoryGridData>(NewInventoryData))
	{
		const TMap<UItemInstance*, FVector2D> ItemCoords = InventoryGridData->GetItemCoords();
		for (UItemInstance* Item : InventoryGridData->GetItems())
		{
			AddItemWidget(Item, ItemCoords.FindRef(Item));
		}
	}
}

bool UInventoryGridWidget::TryAddItem(UItemInstance* ItemToAdd)
{
	return GetInventoryGridData()->TryAddItem(ItemToAdd);
}

void UInventoryGridWidget::TryRemoveItem(UItemInstance* ItemToRemove)
{
	GetInventoryGridData()->TryRemoveItem(ItemToRemove);
}

void UInventoryGridWidget::AddItemWidget(UItemInstance* NewItemInstance, FVector2D Pos)
{
	if (const UInventoryDeveloperSettings* IDS = UInventoryStatics::GetInventoryDeveloperSettings())
	{
		TSubclassOf<UItemWidget> ItemWidgetClass = IDS->DefaultItemWidgetClass;
		check(ItemWidgetClass)
		UItemWidget* ItemWidget = CreateWidget<UItemWidget>(GetWorld(), ItemWidgetClass);
		ItemWidget->InitializeItemWidget(NewItemInstance);
		UCanvasPanelSlot* CPS = InventoryCanvasPanel->AddChildToCanvas(ItemWidget);
		CPS->SetAutoSize(true);
		CPS->SetPosition(Pos * IDS->TileSize);
		ItemWidget->OnItemRemoved.AddDynamic(this, &ThisClass::TryRemoveItem);
		ItemToWidgetMap.Add(NewItemInstance, ItemWidget);
	}
}

void UInventoryGridWidget::RemoveItemWidget(UItemInstance* NewItemInstance, FVector2D Pos)
{
	UUserWidget* ItemWidgetToRemove;
	if (ItemToWidgetMap.RemoveAndCopyValue(NewItemInstance, ItemWidgetToRemove))
	{
		ItemWidgetToRemove->RemoveFromParent();
	}
}

void UInventoryGridWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (const UInventoryDeveloperSettings* InventorySettings = UInventoryStatics::GetInventoryDeveloperSettings())
	{
		SetInventoryBounds(UKismetMathLibrary::Conv_IntPointToVector2D(Dimensions) * InventorySettings->TileSize);
		if (UMaterialInstanceDynamic* MID = InventoryImage->GetDynamicMaterial())
		{
			MID->SetScalarParameterValue(TEXT("TileSize"), InventorySettings->TileSize);
		}
		
	}
}

int32 UInventoryGridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FPaintContext Context(AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	OnPaint(Context);

	if (GetWorld()->IsGameWorld())
	{
		UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
		const UInventoryGridData* InventoryGridData = GetInventoryGridData();
		if (bDrawDropLocation && InventorySubsystem->IsDraggingItem() && !InventoryGridData->IsIndexOccupied(InventoryGridData->TileToIndex(ItemMousePos)))
		{
			if(const UInventoryDeveloperSettings* InventorySettings = UInventoryStatics::GetInventoryDeveloperSettings())
			{
				const FColor Tint = InventoryGridData->IsRoomAvailable(InventorySubsystem->GetDraggedItem(), InventoryGridData->TileToIndex(ItemDragPos)) ? InventorySettings->ValidTileColor : InventorySettings->InvalidTileColor;
				UWidgetBlueprintLibrary::DrawBox(Context, UKismetMathLibrary::Conv_IntPointToVector2D(ItemDragPos) * InventorySettings->TileSize, UKismetMathLibrary::Conv_IntPointToVector2D(InventorySubsystem->GetDraggedItem()->GetDimensions()) * InventorySettings->TileSize, SlateBrushAsset, Tint);
			}
		}
	}
	return Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
}

FReply UInventoryGridWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	if (InventorySubsystem->IsDraggingItem())
	{
		if(const UInventoryDeveloperSettings* InventorySettings = GetDefault<UInventoryDeveloperSettings>())
		{
			const FVector2D GridMousePosition = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
			const FVector2D ItemMousePositionOffset = -1.0f * (UKismetMathLibrary::Conv_IntPointToVector2D(InventorySubsystem->GetDraggedItem()->GetDimensions() * InventorySettings->TileSize) / 2.0f);
			ItemDragPos = ((GridMousePosition + ItemMousePositionOffset) / InventorySettings->TileSize).IntPoint();
			FVector2D Temp = GridMousePosition / InventorySettings->TileSize;
			ItemMousePos = FIntPoint(UKismetMathLibrary::FTrunc(Temp.X), UKismetMathLibrary::FTrunc(Temp.Y));
			
		}
	}
	
	return FReply::Unhandled();
}

FReply UInventoryGridWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	const float WheelDelta = InMouseEvent.GetWheelDelta();
	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	if (InventorySubsystem->IsDraggingItem() && WheelDelta <= -1)
	{
		UItemInstance* DraggedItem = InventorySubsystem->GetDraggedItem();
		UInventoryGridData* InventoryGridDataInstance = GetInventoryGridData();
		int32 ItemDropIndex = InventoryGridDataInstance->TileToIndex(ItemDragPos);
		if (InventoryGridDataInstance->IsRoomAvailable(DraggedItem, ItemDropIndex))
		{
			if (DraggedItem->GetCurrentStack() == 1)
			{
				InventoryGridDataInstance->AddItemAtIndex(DraggedItem, ItemDropIndex);
				InventorySubsystem->ClearItemDragWidget();
			}
			else
			{
				UItemInstance* NewItem = DuplicateObject(DraggedItem, nullptr);
				NewItem->SetStack(1);
				int32 MaxDif;
				DraggedItem->AddStack(-1, MaxDif);
				InventoryGridDataInstance->AddItemAtIndex(NewItem, ItemDropIndex);
			}
		}
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Single Drop"));
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

FReply UInventoryGridWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply =  Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	UInventorySubsystem* InventorySubsystem = UGameplayStatics::GetGameInstance(this)->GetSubsystem<UInventorySubsystem>();
	if (InventorySubsystem->IsDraggingItem() && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UItemInstance* DraggedItem = InventorySubsystem->GetDraggedItem();
		UInventoryGridData* InventoryGridDataInstance = GetInventoryGridData();
		int32 ItemDropIndex = InventoryGridDataInstance->TileToIndex(ItemDragPos);
		if (InventoryGridDataInstance->IsRoomAvailable(DraggedItem, ItemDropIndex))
		{
			InventoryGridDataInstance->AddItemAtIndex(DraggedItem, ItemDropIndex);
		}
		else
		{
			TryAddItem(DraggedItem);
		}
		InventorySubsystem->ClearItemDragWidget();
	}
	return Reply;
}

UInventoryGridData* UInventoryGridWidget::GetInventoryGridData() const
{
	return Cast<UInventoryGridData>(InventoryData);
}
