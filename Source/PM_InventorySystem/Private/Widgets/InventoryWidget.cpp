// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryWidget.h"

#include "Blueprint/WidgetTree.h"
#include "InventoryData/InventoryData.h"
#include "Widgets/CommonInventoryWidget.h"

UInventoryData* UInventoryWidget::CreateNewInventoryData(UObject* Payload)
{
	if (InventoryData)
	{
		return InventoryData;
	}
	return NewObject<UInventoryData>();
}

void UInventoryWidget::SetInventoryData(UInventoryData* NewInventoryData)
{
	InventoryData = NewInventoryData;
}

TArray<UCommonInventoryWidget*> UInventoryWidget::GetItemAddOrder() const
{
	TArray<UWidget*> Children;
	TArray<UCommonInventoryWidget*> ItemAddOrder;
	WidgetTree->GetAllWidgets(Children);
	for(auto Child : Children)
	{
		if(UCommonInventoryWidget* CommonInventory = Cast<UCommonInventoryWidget>(Child))
		{
			// Brute force sort because I can't think of a use-case where there are more than 3 of these in an add order
			// Plus it only runs on demand, not per tick so whatever.
			if (CommonInventory->AddOrderPriority >= 0)
			{
				if (ItemAddOrder.IsEmpty())
				{
					ItemAddOrder.AddUnique(CommonInventory);
					continue;
				}
				
				for (int i = 0; i < ItemAddOrder.Num(); i++)
				{
					if (ItemAddOrder[i]->AddOrderPriority >= CommonInventory->AddOrderPriority)
					{
						ItemAddOrder.Insert(CommonInventory, i);
						break;
					}
					if (i == ItemAddOrder.Num() - 1)
					{
						ItemAddOrder.AddUnique(CommonInventory);
						break;
					}
				}
			}
			
		}
	}

	return ItemAddOrder;
}

bool UInventoryWidget::TryAddItem(UItemInstance* ItemToAdd)
{
	return InventoryData->TryAddItem(ItemToAdd);
}

bool UInventoryWidget::ContainsItem(UItemInstance* InItemInstance)
{
	return InventoryData->ContainsItem(InItemInstance);
}

