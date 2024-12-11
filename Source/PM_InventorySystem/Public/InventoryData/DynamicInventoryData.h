// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData/InventoryData.h"
#include "DynamicInventoryData.generated.h"

class UDynamicInventoryScript;
class UDynamicInventoryWidget;
/**
 * Note: The reason I created Dynamic Inventories is because I want to create inventories that are capable of changing
 * capacities at runtime. For example, if an Item is able to receive multiple attachments and one of those attachments also
 * is able to receive attachments, then I want to be able to interact with all inventory slots of both the base item and its
 * attachment in a single place. 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UDynamicInventoryData : public UInventoryData
{
	GENERATED_BODY()

public:
	void InitializeDynamicInventoryData(UDynamicInventoryWidget* InDynamicInventoryWidget, UObject* Payload);

	UPROPERTY()
	TArray<UInventoryData*> DynamicInventoryDatas;
};
