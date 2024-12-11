// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemInstance.h"
#include "UObject/NoExportTypes.h"
#include "ItemModifier.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, Abstract, EditInLineNew, Blueprintable)
class PM_INVENTORYSYSTEM_API UItemModifier : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void InitializeItemModifier(UItemInstance* InItemInstance, UObject* InWorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemInstance* GetItemReference() { return ItemReference; }

	UPROPERTY()
	UObject* WorldContextObject;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UItemInstance> ItemReference;
};
