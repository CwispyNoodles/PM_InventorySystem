// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryData/DynamicInventoryData.h"

#include "InventoryData/DynamicInventoryInterface.h"
#include "InventoryData/DynamicInventoryScript.h"
#include "InventoryData/InventorySlotData.h"
#include "Items/Modifiers/ItemModifier.h"
#include "Widgets/DynamicInventoryWidget.h"

void UDynamicInventoryData::InitializeDynamicInventoryData(UDynamicInventoryWidget* InDynamicInventoryWidget, UObject* Payload)
{
	TArray<FDynamicInventorySlotHandle> DynamicInventorySlotHandles;
	if (IDynamicInventoryInterface* DII = Cast<IDynamicInventoryInterface>(Payload))
	{
		DII->GetDynamicInventorySlotHandles(DynamicInventorySlotHandles);
	}

	DynamicInventorySlotHandles.Append(InDynamicInventoryWidget->DynamicInventorySlotHandles);

	for (auto Handle : DynamicInventorySlotHandles)
	{
		UInventorySlotData* NewInventorySlotData = NewObject<UInventorySlotData>();
		NewInventorySlotData->InventorySlotHandle = Handle.InventorySlotHandle;
		NewInventorySlotData->SetDimensions(Handle.Dimensions);

		if (IsValid(Handle.SlotScript))
		{
			UDynamicInventoryScript* Script = Handle.SlotScript;
			if (UItemModifier* ItemModifier = Cast<UItemModifier>(Payload))
			{
				Script->ItemInstance = ItemModifier->GetItemReference();
			}

			Script->Initialize();
			
			NewInventorySlotData->OnItemAddedDelegate.AddUObject(Script, &UDynamicInventoryScript::OnItemAdded);
			NewInventorySlotData->OnItemRemovedDelegate.AddUObject(Script, &UDynamicInventoryScript::OnItemRemoved);
		}
		
		DynamicInventoryDatas.Add(NewInventorySlotData);
	}
}
