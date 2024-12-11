// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySet.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryStatics.generated.h"



class UPMAbilitySystemComponent;
class UAttributeSet;
struct FActiveGameplayEffectHandle;
struct FGameplayAbilitySpecHandle;
class UInventoryDeveloperSettings;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UInventoryStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="PM Inventory")
	static const UInventoryDeveloperSettings* GetInventoryDeveloperSettings();
};
