// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryData/DynamicInventoryScript.h"

#include "Items/ItemInstance.h"

UItemInstance* UDynamicInventoryScript::GetItemInstance(bool& bSuccess)
{
	if (IsValid(ItemInstance))
	{
		bSuccess = true;
		return ItemInstance;
	}
	bSuccess = false;
	return nullptr;
}
