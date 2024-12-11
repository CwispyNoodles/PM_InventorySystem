// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InventorySubsystem.generated.h"

class UInventoryHUDComponent;
class UItemDragWidget;
class UItemInstance;
class UItemDefinition;
/**
 * 
 */
UCLASS()
class PM_INVENTORYSYSTEM_API UInventorySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	UItemInstance* CreateItemInstanceFromItemDefinition(UItemDefinition* InItemDefinition, bool& bSuccess, UObject* NewOwner = nullptr);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsDraggingItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemInstance* GetDraggedItem();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryHUDComponent* GetInventoryHUDComponent();
	
	UFUNCTION(BlueprintCallable)
	UItemDragWidget* CreateItemDragWidget(UItemInstance* ItemToDrag);
	
	UFUNCTION(BlueprintCallable)
	void ClearItemDragWidget();
};
