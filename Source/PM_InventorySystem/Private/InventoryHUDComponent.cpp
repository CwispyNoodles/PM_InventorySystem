// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryHUDComponent.h"

#include "InventoryDeveloperSettings.h"
#include "InventoryStatics.h"
#include "InventoryVerbMessage.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Items/ItemDefinition.h"
#include "Items/ItemInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Widgets/InventoryHUDWidget.h"
#include "Widgets/ItemDragWidget.h"
#include "Widgets/ItemWidget.h"

// Sets default values for this component's properties
UInventoryHUDComponent::UInventoryHUDComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UInventoryHUDComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryHUDComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryHUDComponent::IsDraggingItem() const
{
	return (DraggedItem != nullptr);
}

UItemDragWidget* UInventoryHUDComponent::CreateItemDragWidget(UItemInstance* ItemToDrag)
{
	if (!IsDraggingItem())
	{
		DraggedItem = ItemToDrag;
		const UInventoryDeveloperSettings* InventorySettings = UInventoryStatics::GetInventoryDeveloperSettings();
		UItemDragWidget* ItemDragWidget = CreateWidget<UItemDragWidget>(UGameplayStatics::GetGameInstance(this), InventorySettings->DefaultItemDragWidgetClass);
		ItemDragWidgetInstance = ItemDragWidget;
		ItemDragWidget->AddToViewport(99999);
		ItemDragWidget->InitializeItemDragWidget(ItemToDrag);

		UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
		FInventoryVerbMessage VerbMessage;
		VerbMessage.InstigatorTags.AddTag(ItemToDrag->GetItemDefinition()->ItemTag);
		VerbMessage.Instigator = ItemToDrag;
		MessageSubsystem.BroadcastMessage(TAG_Message_UI_ItemWidget_Dragged, VerbMessage);
	}
	return nullptr;
}

void UInventoryHUDComponent::ClearItemDragWidget()
{
	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(this);
	FInventoryVerbMessage VerbMessage;
	VerbMessage.InstigatorTags.AddTag(DraggedItem->GetItemDefinition()->ItemTag);
	VerbMessage.Instigator = DraggedItem;
	
	DraggedItem = nullptr;
	ItemDragWidgetInstance->RemoveFromParent();
	ItemDragWidgetInstance = nullptr;
	
	MessageSubsystem.BroadcastMessage(TAG_Message_UI_ItemWidget_Dropped, VerbMessage);
}

UInventoryHUDWidget* UInventoryHUDComponent::OpenInventory(bool& bSuccess)
{
	if (InventoryHUDWidgetClass)
	{
		InventoryHUDWidgetInstance = CreateWidget<UInventoryHUDWidget>(UGameplayStatics::GetGameInstance(this), InventoryHUDWidgetClass);
		InventoryHUDWidgetInstance->AddToViewport();
		bSuccess = true;
		return InventoryHUDWidgetInstance;
	}
	bSuccess = false;
	return nullptr;
}

void UInventoryHUDComponent::CloseInventory()
{
	InventoryHUDWidgetInstance->RemoveFromParent();
	InventoryHUDWidgetInstance = nullptr;
}

UInventoryHUDWidget* UInventoryHUDComponent::GetInventory(bool& bIsValid)
{
	bIsValid = IsValid(InventoryHUDWidgetInstance);
	return InventoryHUDWidgetInstance;
}

bool UInventoryHUDComponent::IsInventoryOpen()
{
	return IsValid(InventoryHUDWidgetInstance);
}

