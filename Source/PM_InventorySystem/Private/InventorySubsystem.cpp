// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySubsystem.h"

#include "InventoryHUDComponent.h"
#include "GameFramework/HUD.h"
#include "Items/ItemInstance.h"
#include "Kismet/GameplayStatics.h"

UItemInstance* UInventorySubsystem::CreateItemInstanceFromItemDefinition(UItemDefinition* InItemDefinition, bool& bSuccess, UObject* NewOwner)
{
	UItemInstance* NewItemInstance = NewObject<UItemInstance>();
	if (NewItemInstance->InitializeItemInstance(InItemDefinition, GetGameInstance(), NewOwner))
	{
		bSuccess = true;
		return NewItemInstance;
	}
	bSuccess = false;
	return nullptr;
}

bool UInventorySubsystem::IsDraggingItem()
{
	return GetInventoryHUDComponent()->IsDraggingItem();
}

UItemInstance* UInventorySubsystem::GetDraggedItem()
{
	return GetInventoryHUDComponent()->GetDraggedItem();
}

UInventoryHUDComponent* UInventorySubsystem::GetInventoryHUDComponent()
{
	AHUD* HUD = UGameplayStatics::GetPlayerController(GetGameInstance(), 0)->GetHUD();
	return HUD->GetComponentByClass<UInventoryHUDComponent>();
}

UItemDragWidget* UInventorySubsystem::CreateItemDragWidget(UItemInstance* ItemToDrag)
{
	UItemDragWidget* ItemDragWidget = GetInventoryHUDComponent()->CreateItemDragWidget(ItemToDrag);
	return ItemDragWidget;
}

void UInventorySubsystem::ClearItemDragWidget()
{
	GetInventoryHUDComponent()->ClearItemDragWidget();
}