// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DynamicInventoryInterface.generated.h"

struct FDynamicInventorySlotHandle;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDynamicInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PM_INVENTORYSYSTEM_API IDynamicInventoryInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void GetDynamicInventorySlotHandles(TArray<FDynamicInventorySlotHandle>& InArray) = 0;
};
