// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "InventoryManagerComponent.generated.h"


class UInventorySlotData;
class UItemInstance;
class UInventoryData;
class UInventoryWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PM_INVENTORYSYSTEM_API UInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryManagerComponent();

	// UActorComponent Interface
	virtual void InitializeComponent() override;
	// end of UActorComponent Interface

	// Attempts to add item to associated inventory data.
	// Initializes inventory if it hasn't already done so.
	UFUNCTION(BlueprintCallable)
	bool TryAddItem(UItemInstance* InItemInstance);
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventorySlotData* GetInventorySlotDataByTag(FGameplayTag InventorySlotTag, bool& bSuccess);

	// If set to false, this inventory will not automatically initialize.
	// should be initialized on demand. 
	// typically only want to auto initialize for player(s). 
	UPROPERTY(EditDefaultsOnly)
	bool bAutoInitialization = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UInventoryData* GetInventoryData() { return InventoryData; }

	UFUNCTION(BlueprintCallable)
	void UnequipItem();

	UFUNCTION(BlueprintCallable)
	bool EquipItemByItemInstance(UItemInstance* ItemToEquip);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void InitializeInventory();
	
	UPROPERTY()
	TObjectPtr<UInventoryData> InventoryData;
	
	UPROPERTY()
	bool bInitialized = false;

		
};
