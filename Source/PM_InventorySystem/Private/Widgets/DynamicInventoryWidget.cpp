// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DynamicInventoryWidget.h"

#include "Components/WrapBox.h"
#include "InventoryData/CommonInventoryData.h"
#include "InventoryData/DynamicInventoryData.h"

void UDynamicInventoryWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	// Refresh();
}

bool UDynamicInventoryWidget::TryAddItem(UItemInstance* ItemToAdd)
{
	return Super::TryAddItem(ItemToAdd);
}

void UDynamicInventoryWidget::TryRemoveItem(UItemInstance* ItemToRemove)
{
	Super::TryRemoveItem(ItemToRemove);
}

UInventoryData* UDynamicInventoryWidget::CreateNewInventoryData(UObject* Payload)
{
	if (InventoryData)
	{
		return InventoryData;
	}
	UDynamicInventoryData* NewDynamicInventoryData = NewObject<UDynamicInventoryData>();
	SetInventoryData(NewDynamicInventoryData);
	NewDynamicInventoryData->InitializeDynamicInventoryData(this, Payload);
	return NewDynamicInventoryData;
}

void UDynamicInventoryWidget::SetInventoryData(UInventoryData* NewInventoryData)
{
	Super::SetInventoryData(NewInventoryData);

	Refresh();
}

void UDynamicInventoryWidget::Refresh()
{
	if (!EntryWidgetClass)
	{
		return;
	}

	Container->ClearChildren();
	
	if (UDynamicInventoryData* DynamicInventoryData = Cast<UDynamicInventoryData>(InventoryData))
	{
		for (auto InventorySlotData : DynamicInventoryData->DynamicInventoryDatas)
		{
			UInventorySlotWidget* EntryWidget = CreateWidget<UInventorySlotWidget>(GetWorld(), EntryWidgetClass);
			EntryWidget->SetInventoryData(InventorySlotData);
			if (UCommonInventoryData* CommonInventoryData = Cast<UCommonInventoryData>(InventorySlotData))
			{
				CommonInventoryData->OnItemAddedDelegate.AddUObject(EntryWidget, &UCommonInventoryWidget::AddItemWidget);
				CommonInventoryData->OnItemRemovedDelegate.AddUObject(EntryWidget, &UCommonInventoryWidget::RemoveItemWidget);
			}
			
			Container->AddChild(EntryWidget);
		}
	}
}

void UDynamicInventoryWidget::AddDynamicInventorySlotHandles(TArray<FDynamicInventorySlotHandle> InHandles)
{
	if (!InHandles.IsEmpty())
	{
		DynamicInventorySlotHandles.Append(InHandles);
	}
	Refresh();
}
