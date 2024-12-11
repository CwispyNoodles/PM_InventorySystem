// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryManagerComponent.h"

#include "InventoryData/InventoryData.h"
#include "Widgets/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryManagerComponent::UInventoryManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

	// ...
}

void UInventoryManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (bAutoInitialization)
	{
		InitializeInventory();
	}
}

bool UInventoryManagerComponent::TryAddItem(UItemInstance* InItemInstance)
{
	if (!bInitialized)
	{
		InitializeInventory();
	}
	return InventoryData->TryAddItem(InItemInstance);
}

UInventorySlotData* UInventoryManagerComponent::GetInventorySlotDataByTag(FGameplayTag InventorySlotTag, bool& bSuccess)
{
	return InventoryData->GetInventorySlotDataByTag(InventorySlotTag, bSuccess);
}

void UInventoryManagerComponent::InitializeInventory()
{
	check(InventoryWidgetClass)
	InventoryData = NewObject<UInventoryData>(this);
	InventoryData->InitializeInventoryData(InventoryWidgetClass, GetOwner()); 
	bInitialized = true;
}

void UInventoryManagerComponent::UnequipItem()
{
}

bool UInventoryManagerComponent::EquipItemByItemInstance(UItemInstance* ItemToEquip)
{
	return false;
}

// Called when the game starts
void UInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
