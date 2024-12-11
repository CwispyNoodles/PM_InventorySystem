// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PMGameplayAbility.h"
#include "ItemAbility.generated.h"

class UInventoryData;
class UItemInstance;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UItemAbility : public UPMGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemInstance* GetAssociatedItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryData* GetOwnerInventoryData(bool& bSuccess);
};
