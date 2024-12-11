// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryData/InventoryData.h"
#include "CommonInventoryData.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemHandledSignature, UItemInstance*, FVector2D)

/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UCommonInventoryData : public UInventoryData
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<UItemInstance*> GetItems() { return Items; }

	UFUNCTION(BlueprintCallable)
	void SetDimensions(FVector2D NewDimensions) { Dimensions = NewDimensions; }

	FOnItemHandledSignature OnItemAddedDelegate;
	FOnItemHandledSignature OnItemRemovedDelegate;
protected:
	UPROPERTY()
	TArray<UItemInstance*> Items;

	UPROPERTY()
	FVector2D Dimensions;
};
