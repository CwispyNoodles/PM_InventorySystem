// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DynamicInventoryScript.generated.h"

class UItemInstance;
/**
 * 
 */
UCLASS(Blueprintable, Abstract, EditInLineNew, Blueprintable)
class PM_INVENTORYSYSTEM_API UDynamicInventoryScript : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Initialize();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnItemAdded(UItemInstance* ItemAdded, FVector2D Location);

	UFUNCTION(BlueprintImplementableEvent)
	void OnItemRemoved(UItemInstance* ItemRemoved, FVector2D Location);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemInstance* GetItemInstance(bool& bSuccess);
	
	UPROPERTY()
	TObjectPtr<UItemInstance> ItemInstance;
};
