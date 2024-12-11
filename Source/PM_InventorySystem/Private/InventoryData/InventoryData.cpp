// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryData/InventoryData.h"

#include "Blueprint/WidgetTree.h"
#include "InventoryData/CommonInventoryData.h"
#include "InventoryData/DynamicInventoryData.h"
#include "InventoryData/InventorySlotData.h"
#include "Items/ItemInstance.h"
#include "Widgets/CommonInventoryWidget.h"
#include "Widgets/InventoryWidget.h"

void UInventoryData::InitializeInventoryData(TSubclassOf<UInventoryWidget> InInventoryWidgetClass, UObject* InWorldContextObject, UObject* Payload)
{
	check(InInventoryWidgetClass)
	InventoryWidgetClass = InInventoryWidgetClass;

	check(InWorldContextObject)
	SetNewOwner(InWorldContextObject);

	UInventoryWidget* TempInventoryWidgetInstance = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	// TempInventoryWidgetInstance->InitializeInventoryWidget(this);
	const TArray<UCommonInventoryWidget*> TempAddOrder = TempInventoryWidgetInstance->GetItemAddOrder();

	for (const auto Curr : TempAddOrder)
	{
		ItemAddOrder.AddUnique(Curr->CreateNewInventoryData(Payload));
	}

	TArray<UWidget*> Children;
	TempInventoryWidgetInstance->WidgetTree->GetAllWidgets(Children);

	for(const auto Child : Children)
	{
		if (UInventoryWidget* ChildInventoryWidget = Cast<UInventoryWidget>(Child))
		{
			UInventoryData* NewInventoryData = ChildInventoryWidget->CreateNewInventoryData(Payload);
			NewInventoryData->SetNewOwner(InWorldContextObject);
			ChildInventories.Add(ChildInventoryWidget->GetName(), NewInventoryData);

			if (UInventorySlotData* ISD = Cast<UInventorySlotData>(NewInventoryData))
			{
				InventorySlotData.AddUnique(ISD);
			}
		}
	}
	
	// UInventoryWidget* TempInventoryWidgetInstance = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	// const TArray<UCommonInventoryWidget*> TempAddOrder = TempInventoryWidgetInstance->GetItemAddOrder();
	// ItemAddOrder.SetNum(TempAddOrder.Num());
	// for( TFieldIterator<FObjectProperty> Prop(InventoryWidgetClass); Prop; ++Prop ) {
	// 	if (Prop->PropertyClass->IsChildOf(UCommonInventoryWidget::StaticClass()))
	// 	{
	// 		const void* ValueAddress = Prop->ContainerPtrToValuePtr< void >( TempInventoryWidgetInstance );
	// 		if (UCommonInventoryWidget* ChildInventoryWidget = Cast<UCommonInventoryWidget>(Prop->GetObjectPropertyValue( ValueAddress )))
	// 		{
	// 			UCommonInventoryData* NewInventoryData = ChildInventoryWidget->CreateNewInventoryData();
	// 			NewInventoryData->SetNewOwner(NewOwner);
	// 			
	// 			ChildInventories.Add(ChildInventoryWidget->GetName(), NewInventoryData);
	// 			if (TempAddOrder.Contains(ChildInventoryWidget))
	// 			{
	// 				ItemAddOrder[TempAddOrder.Find(ChildInventoryWidget)] = NewInventoryData;
	// 			}
	// 		}
	// 	}
	// }
}

UInventoryWidget* UInventoryData::CreateInventoryWidgetInstance()
{
	UInventoryWidget* InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
	InventoryWidget->SetInventoryData(this);
	TArray<UWidget*> Children;
	InventoryWidget->WidgetTree->GetAllWidgets(Children);
	for(const auto Child : Children)
	{
		if (UInventoryWidget* ChildInventoryWidget = Cast<UInventoryWidget>(Child))
		{
			UInventoryData* InventoryData = ChildInventories.FindRef(ChildInventoryWidget->GetName());
			ChildInventoryWidget->SetInventoryData(InventoryData);
			
			if (UCommonInventoryData* CommonInventoryData = Cast<UCommonInventoryData>(InventoryData))
			{
				UCommonInventoryWidget* CommonInventoryWidget = Cast<UCommonInventoryWidget>(ChildInventoryWidget);
				CommonInventoryData->OnItemAddedDelegate.AddUObject(CommonInventoryWidget, &UCommonInventoryWidget::AddItemWidget);
				CommonInventoryData->OnItemRemovedDelegate.AddUObject(CommonInventoryWidget, &UCommonInventoryWidget::RemoveItemWidget);
			}

			if (UDynamicInventoryData* DynamicInventoryData = Cast<UDynamicInventoryData>(InventoryData))
			{
				
			}
		}
	}
	// for( TFieldIterator<FObjectProperty> Prop(InventoryWidgetClass); Prop; ++Prop )
	// {
	// 	if (Prop->PropertyClass->IsChildOf(UCommonInventoryWidget::StaticClass()))
	// 	{
	// 		const void* ValueAddress = Prop->ContainerPtrToValuePtr< void >( InventoryWidget );
	// 		if (UCommonInventoryWidget* ConnectedInventoryWidget = Cast<UCommonInventoryWidget>(Prop->GetObjectPropertyValue( ValueAddress )))
	// 		{
	// 			UCommonInventoryData* CommonInventoryData = ChildInventories.FindRef(ConnectedInventoryWidget->GetName());
	// 			ConnectedInventoryWidget->SetInventoryData(CommonInventoryData);
	// 			CommonInventoryData->OnItemAddedDelegate.AddUObject(ConnectedInventoryWidget, &UCommonInventoryWidget::AddItemWidget);
	// 			CommonInventoryData->OnItemRemovedDelegate.AddUObject(ConnectedInventoryWidget, &UCommonInventoryWidget::RemoveItemWidget);
	// 		}
	// 	}
	// }
	return InventoryWidget;
}

bool UInventoryData::TryAddItem(UItemInstance* InItemInstance)
{
	check(InItemInstance)
	for (auto ChildInventory : ItemAddOrder)
	{
		if (ChildInventory->TryAddItem(InItemInstance))
		{
			InItemInstance->SetOwner(Owner);
			return true;
		}
	}
	return false;
}

bool UInventoryData::TryRemoveItem(UItemInstance* ItemToRemove)
{
	TArray<UInventoryData*> Inventories;
	ChildInventories.GenerateValueArray(Inventories);
	
	for (UInventoryData* Inventory : Inventories)
	{
		if (Inventory->ContainsItem(ItemToRemove))
		{
			ItemToRemove->RemoveDelegate.Clear();
			Inventory->TryRemoveItem(ItemToRemove);
			ItemToRemove->SetOwner(nullptr);
			return true;
		}
	}
	return false;
}

void UInventoryData::FindItemByTagWithSmallestStackSize(FGameplayTag InItemTag, UItemInstance*& InItemInstance,
	bool& bSuccess)
{
	TArray<UInventoryData*> Inventories;
	ChildInventories.GenerateValueArray(Inventories);

	for (UInventoryData* Inventory : Inventories)
	{
		Inventory->FindItemByTagWithSmallestStackSize(InItemTag,InItemInstance,bSuccess);
	}

	bSuccess = IsValid(InItemInstance);
}

void UInventoryData::FindItemsByTag(FGameplayTag InItemTag, TArray<UItemInstance*>& InArray, bool bSortAscending)
{
	TArray<UInventoryData*> Inventories;
	ChildInventories.GenerateValueArray(Inventories);

	for (UInventoryData* Inventory : Inventories)
	{
		Inventory->FindItemsByTag(InItemTag, InArray, bSortAscending);
	}
}

UInventorySlotData* UInventoryData::GetInventorySlotDataByTag(FGameplayTag InventorySlotTag, bool& bSuccess)
{
	for (UInventorySlotData* ISD : InventorySlotData)
	{
		if (ISD->InventorySlotHandle.EquipmentSlotTag == InventorySlotTag)
		{
			bSuccess = true;
			return ISD;
		}
	}
	bSuccess = false;
	return nullptr;
}

bool UInventoryData::ContainsItem(UItemInstance* InItemInstance)
{
	for (UInventoryData* ChildInventory : GetChildInventoriesArray())
	{
		if (ChildInventory->ContainsItem(InItemInstance))
		{
			return true;
		}
	}
	return false;
}

TArray<UInventoryData*> UInventoryData::GetChildInventoriesArray()
{
	TArray<UInventoryData*> Vals;
	ChildInventories.GenerateValueArray(Vals);
	return Vals;
}

void UInventoryData::SetNewOwner(UObject* NewOwner)
{
	check(NewOwner->GetWorld())
	Owner = NewOwner;
}

UWorld* UInventoryData::GetWorld() const
{
	check(Owner)
	return Owner->GetWorld();
}
